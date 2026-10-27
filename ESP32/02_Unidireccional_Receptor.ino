/*
  PRACTICA 2 - Comunicacion UNIDIRECCIONAL - RECEPTOR
  Esta placa RECIBE los datos del emisor.
  NO necesitas cambiar nada ni escribir ninguna MAC.
  Subelo primero, abre el Serial Monitor (115200) y espera los mensajes.
*/
#include <WiFi.h>
#include <esp_now.h>

typedef struct __attribute__((packed)) {
  uint32_t contador;
  float valor;
  char texto[32];
} Paquete;

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  if (len != sizeof(Paquete)) return;
  Paquete p;
  memcpy(&p, data, sizeof(p));
  p.texto[sizeof(p.texto) - 1] = '\0';
  Serial.printf("Recibido #%lu | valor=%.1f | \"%s\" | rssi=%d dBm\n",
                (unsigned long)p.contador, p.valor, p.texto, info->rx_ctrl->rssi);
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

void loop() {}
