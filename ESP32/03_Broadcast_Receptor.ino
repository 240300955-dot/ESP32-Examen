/*
  PRACTICA 3 - BROADCAST - RECEPTOR
  Recibe los mensajes broadcast de cualquier emisor cercano. NO necesita ninguna MAC.
  No cambies nada. Muestra la MAC de quien envio el mensaje.
*/
#include <WiFi.h>
#include <esp_now.h>

typedef struct __attribute__((packed)) {
  uint32_t contador;
  char texto[32];
} Paquete;

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  if (len != sizeof(Paquete)) return;
  Paquete p;
  memcpy(&p, data, sizeof(p));
  p.texto[sizeof(p.texto) - 1] = '\0';
  const uint8_t *m = info->src_addr;
  Serial.printf("De %02X:%02X:%02X:%02X:%02X:%02X | #%lu | \"%s\" | rssi=%d dBm\n",
                m[0], m[1], m[2], m[3], m[4], m[5],
                (unsigned long)p.contador, p.texto, info->rx_ctrl->rssi);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error al iniciar ESP-NOW");
    while (true) delay(1000);
  }
  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("Receptor broadcast listo");
}

void loop() {}
