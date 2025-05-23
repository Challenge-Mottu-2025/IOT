#include <WiFi.h>
#include <WebServer.h>

// === Configurações Wi-Fi ===
const char* ssid = "SEU_WIFI";
const char* password = "SENHA_WIFI";

WebServer server(80);

// === Trilateração ===
struct AccessPoint {
  const char* ssid;
  float x, y;
  int rssi;
  float distance;
};

AccessPoint aps[3] = {
  {"AP1", 0.0, 0.0, 0, 0},
  {"AP2", 5.0, 0.0, 0, 0},
  {"AP3", 2.5, 4.33, 0, 0}
};

const int RSSI_REF = -40;                
const float PATH_LOSS_EXPONENT = 2.5;   

float calculateDistance(int rssi) {
  return pow(10.0, ((RSSI_REF - rssi) / (10.0 * PATH_LOSS_EXPONENT)));
}

bool trilaterate(float& x, float& y) {
  float x1 = aps[0].x, y1 = aps[0].y, r1 = aps[0].distance;
  float x2 = aps[1].x, y2 = aps[1].y, r2 = aps[1].distance;
  float x3 = aps[2].x, y3 = aps[2].y, r3 = aps[2].distance;

  float A_coef = 2 * (x2 - x1);
  float B_coef = 2 * (y2 - y1);
  float C_coef = r1 * r1 - r2 * r2 - x1 * x1 + x2 * x2 - y1 * y1 + y2 * y2;
  float D_coef = 2 * (x3 - x2);
  float E_coef = 2 * (y3 - y2);
  float F_coef = r2 * r2 - r3 * r3 - x2 * x2 + x3 * x3 - y2 * y2 + y3 * y3;

  float denominator = (A_coef * E_coef - D_coef * B_coef);
  if (denominator == 0) {
    return false; 
  }

  x = (C_coef * E_coef - F_coef * B_coef) / denominator;
  y = (A_coef * F_coef - D_coef * C_coef) / denominator;
  return true;
}

void handleRoot() {
  Serial.println("Iniciando scan de redes WiFi...");
  int networks = WiFi.scanNetworks();

  if (networks == 0) {
    Serial.println("Nenhuma rede encontrada.");
    server.send(200, "text/html", "<p>Erro: Nenhuma rede WiFi encontrada.</p>");
    return;
  }

  // Zera os RSSIs antes de buscar
  for (int i = 0; i < 3; i++) {
    aps[i].rssi = -1000; 
  }

  // Para cada AP procurado, busca o RSSI no resultado do scan
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < networks; j++) {
      if (WiFi.SSID(j) == aps[i].ssid) {
        aps[i].rssi = WiFi.RSSI(j);
        break;
      }
    }
    if (aps[i].rssi == -1000) {
      Serial.printf("⚠️ AP \"%s\" não encontrado no scan.\n", aps[i].ssid);
    } else {
      aps[i].distance = calculateDistance(aps[i].rssi);
      Serial.printf("AP \"%s\": RSSI = %d dBm, distância estimada = %.2f m\n",
                    aps[i].ssid, aps[i].rssi, aps[i].distance);
    }
  }

  // Verifica se encontrou todos os APs
  bool allFound = true;
  for (int i = 0; i < 3; i++) {
    if (aps[i].rssi == -1000) {
      allFound = false;
      break;
    }
  }

  String html = "<!DOCTYPE html><html><head><meta charset='utf-8'><title>Localização ESP32</title>";
  html += "<meta http-equiv='refresh' content='5'>";
  html += "<style>body{font-family:sans-serif;text-align:center;padding-top:50px;}</style></head><body>";
  html += "<h1>📍 Posição Estimada</h1>";

  if (!allFound) {
    html += "<p><strong>Erro:</strong> Nem todos os APs foram encontrados no scan.</p>";
  } else {
    float x, y;
    if (trilaterate(x, y)) {
      html += "<p><strong>X:</strong> " + String(x, 2) + " m</p>";
      html += "<p><strong>Y:</strong> " + String(y, 2) + " m</p>";
    } else {
      html += "<p>Erro ao calcular posição (divisão por zero).</p>";
    }
  }

  html += "<p><em>Atualiza a cada 5 segundos</em></p>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ Conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}
