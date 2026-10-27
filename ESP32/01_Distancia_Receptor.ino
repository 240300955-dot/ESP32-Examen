/*
  PRACTICA 1 - Distancia maxima - RECEPTOR
  NO necesita ninguna MAC. Subelo a la placa que se aleja con la computadora.
  Imprime una linea por paquete con el RSSI y los paquetes perdidos.
*/
#include <WiFi.h>
#include <esp_now.h>

typedef struct __attribute__((packed)) {
  uint32_t seq;
} Paquete;

uint32_t recibidos = 0, perdidos = 0, ultimaSeq = 0;
int rssi = 0;
volatile bool nuevo = false;
bool primero = true;

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  if (len != sizeof(Paquete)) return;
  Paquete p;
  memcpy(&p, data, sizeof(p));
  if (primero || p.seq < ultimaSeq) {      // primer paquete o el emisor se reinicio
    primero = false;
    recibidos = 0;
    perdidos = 0;
  } else if (p.seq > ultimaSeq + 1) {
    perdidos += p.seq - ultimaSeq - 1;
  }
  ultimaSeq = p.seq;
  recibidos++;
  rssi = info->rx_ctrl->rssi;
  nuevo = true;
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
  Serial.println("Receptor listo");
}

void loop() {
  if (nuevo) {
    nuevo = false;
    float pct = 100.0f * perdidos / (recibidos + perdidos);
    Serial.printf("seq=%lu, rssi=%d dBm, recibidos=%lu, perdidos=%lu, perdida=%.1f%%\n",
                  (unsigned long)ultimaSeq, rssi, (unsigned long)recibidos,
                  (unsigned long)perdidos, pct);
  }
}
