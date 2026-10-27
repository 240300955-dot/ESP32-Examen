/*
  PRACTICA 3 - BROADCAST - EMISOR
  Envia un mensaje a TODAS las placas cercanas. NO necesita ninguna MAC.
  No cambies nada. Nota: en broadcast no hay ACK, el "enviado" no garantiza que alguien lo recibio.
*/
#include <WiFi.h>
#include <esp_now.h>

const uint8_t BROADCAST_MAC[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

typedef struct __attribute__((packed)) {
  uint32_t contador;
  char texto[32];
} Paquete;

uint32_t contador = 0;

void OnDataSent(const void *info, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "  -> enviado al aire (sin ACK en broadcast)"
                                                : "  -> error al enviar");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error al iniciar ESP-NOW");
    while (true) delay(1000);
  }
  esp_now_register_send_cb((esp_now_send_cb_t)OnDataSent);

  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, BROADCAST_MAC, 6);
  peer.channel = 0;
  peer.ifidx = WIFI_IF_STA;
  peer.encrypt = false;          // broadcast no puede ir cifrado
  esp_now_add_peer(&peer);
  Serial.println("Emisor broadcast listo");
}

void loop() {
  Paquete p;
  p.contador = contador++;
  strncpy(p.texto, "Mensaje para todos", sizeof(p.texto));
  Serial.printf("Broadcast #%lu\n", (unsigned long)p.contador);
  esp_now_send(BROADCAST_MAC, (uint8_t *)&p, sizeof(p));
  delay(2000);
}
