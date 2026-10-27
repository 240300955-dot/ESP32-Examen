/*
  PRACTICA 1 - Distancia maxima - EMISOR
  NO necesita ninguna MAC: envia por broadcast (a todas las placas cercanas).
  Subelo a la placa que se queda quieta. No cambies nada.
*/
#include <WiFi.h>
#include <esp_now.h>

typedef struct __attribute__((packed)) {
  uint32_t seq;
} Paquete;

const uint8_t BROADCAST_MAC[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint32_t seq = 0;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error al iniciar ESP-NOW");
    while (true) delay(1000);
  }
  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, BROADCAST_MAC, 6);
  peer.channel = 0;
  peer.ifidx = WIFI_IF_STA;
  peer.encrypt = false;
  esp_now_add_peer(&peer);
  Serial.println("Emisor listo (enviando 5 paquetes por segundo)");
}

void loop() {
  Paquete p = { seq++ };
  esp_err_t r = esp_now_send(BROADCAST_MAC, (uint8_t *)&p, sizeof(p));
  if (seq % 25 == 0) {
    Serial.printf("Enviados: %lu (ultimo envio: %s)\n", (unsigned long)seq, r == ESP_OK ? "ok" : "error");
  }
  delay(200);
}
