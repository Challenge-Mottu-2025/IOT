# Organizador Mottu

## Descrição do Projeto

O **Organizador Mottu** é um sistema IoT para rastreamento e monitoramento de motos em pátios, 
utilizando ESP32 e Arduino. O objetivo principal é facilitar o controle da localização das motos 
e a ocupação das vagas, ajudando os funcionários a gerenciar o espaço de forma mais eficiente.

O sistema possui duas funcionalidades principais:

- **Triangulação de sinal WiFi com ESP32:** Utilizando múltiplos ESP32 distribuídos nas motos, o sistema realiza a triangulação do sinal WiFi emitido pelas motos, permitindo estimar a posição delas dentro do espaço monitorado.
- **Sensor de vagas com Arduino:** Um sensor conectado a um Arduino detecta a presença de motos nas vagas do patio, acendendo uma luz sempre que a vaga for ocupada.

## Estrutura do Projeto

O projeto está dividido em dois módulos principais:

### 1. esp_triangulacao

- Contém o código do esp32 (`triangulacao.ino`) que realizam a triangulação do sinal WiFi sem retorno HTTP.
- Inclui um arquivo `diagram.json` que representa o diagrama utilizado no Wokwi.
- Funciona coletando sinais WiFi das motos e enviando dados para processamento.

### 2. esp_triangulacao_http

- Contém outro código para esp32 (`triangulacao.ino`) que atua como servidor HTTP, recebendo dados dos ESPs e disponibilizando informações para consulta ou integração.
- Possivelmente atua como interface entre os sensores de triangulação e a rede local ou sistema central.

### 3. sensor_arduino

- Inclui o código Arduino (`sensor.ino`) para o sensor de monitoramento das vagas de motos.
- Possui um arquivo `diagram.json` indicando o diagrama utilizado no Wokwi.
- Detecta a presença de motos nas vagas e acendendo uma luz sempre que a vaga for ocupada.

## Hardware Utilizado

- **ESP32:** Microcontrolador com capacidade WiFi usado para capturar sinais e realizar triangulação.
- **Arduino:** Microcontrolador utilizado para integração com sensores físicos que detectam ocupação das vagas.
- **Sensores:** Sensores de presença conectados ao Arduino para monitoramento das vagas.

## Funcionamento Geral

1. **Detecção do sinal WiFi das motos:** Os ESP32 espalhados pelo pátio captam sinais WiFi emitidos pelas motos (via algum dispositivo instalado nelas ou módulos WiFi das motos).
2. **Triangulação:** Com a intensidade dos sinais recebidos em diferentes pontos, o sistema calcula a posição aproximada das motos dentro do pátio.
3. **Monitoramento das vagas:** O Arduino com sensores detecta a presença física das motos nas vagas específicas, informando a ocupação em tempo real.

## Benefícios

- Melhora a organização do pátio ao permitir a localização rápida das motos.
- Facilita o controle de vagas, evitando ocupações indevidas ou vagas ociosas.
- Automatiza o processo, reduzindo a necessidade de inspeção manual por funcionários.


---

