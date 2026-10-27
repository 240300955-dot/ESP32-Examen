/*
  PRACTICA 4 - BIDIRECCIONAL - TU PLACA (la de dc:da:0c:26:c3:74)
  Esta placa envia y recibe a la vez.
  CAMBIA SOLO macOtraPlaca: pon la MAC de la placa de tu companero.
*/
#include <WiFi.h>
#include <esp_now.h>

// >>> MAC de la OTRA placa <<<
uint8_t macOtraPlaca[6] = {0xB8, 0x1F, 0x3F, 0xDA, 0x18, 0xA8};

typedef struct __attribute__((packed)) {
  uint32_t contador;
  char texto[32];
} Paquete;

uint32_t contador = 0;

void OnDataSent(const void *info, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "  -> entregado (ACK)" : "  -> FALLO (sin ACK)");
}

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  if (len != sizeof(Paquete)) return;
  Paquete p;
  memcpy(&p, data, sizeof(p));
  p.texto[sizeof(p.texto) - 1] = '\0';
  Serial.printf("<< Recibido #%lu \"%s\" (rssi=%d dBm)\n",
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
  esp_now_register_send_cb((esp_now_send_cb_t)OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, macOtraPlaca, 6);
  peer.channel = 0;
  peer.ifidx = WIFI_IF_STA;
  peer.encrypt = false;
  if (esp_now_add_peer(&peer) != ESP_OK) Serial.println("No se pudo agregar el peer");

  Serial.print("Mi MAC: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
  Paquete p;
  p.contador = contador++;
  snprintf(p.texto, sizeof(p.texto), "Hola, soy %s", WiFi.macAddress().c_str());
  Serial.printf(">> Enviando #%lu\n", (unsigned long)p.contador);
  esp_now_send(macOtraPlaca, (uint8_t *)&p, sizeof(p));
  delay(3000);
}
