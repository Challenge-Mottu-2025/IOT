#include <WiFi.h>

// === Nome e senha da rede da moto ===
const char* ssid = "Moto001";  // Nome do "sinal da moto"
const char* password = "12345678"; // Senha opcional

void setup() {
  Serial.begin(115200);

  // Configura o ESP32 como ponto de acesso (Access Point)
  WiFi.softAP(ssid, password);

  Serial.println("🚀 Moto emitindo sinal Wi-Fi...");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("IP da Moto: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  // A moto apenas mantém o sinal emitido, sem fazer mais nada por enquanto.
}
