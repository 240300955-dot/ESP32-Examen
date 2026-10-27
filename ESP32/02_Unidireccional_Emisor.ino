/*
  PRACTICA 2 - Comunicacion UNIDIRECCIONAL - EMISOR
  Esta placa ENVIA datos a la placa receptora.
  CAMBIA SOLO LA LINEA macReceptor: pon la MAC de la placa RECEPTORA.
  Ejemplo: la MAC dc:da:0c:26:c3:74 se escribe {0xDC, 0xDA, 0x0C, 0x26, 0xC3, 0x74}
*/
#include <WiFi.h>
#include <esp_now.h>

// >>> CAMBIA ESTO: MAC de la placa RECEPTORA <<<
uint8_t macReceptor[6] = {0xB8, 0x1F, 0x3F, 0xDA, 0x18, 0xA8};

typedef struct __attribute__((packed)) {
  uint32_t contador;
  float valor;
  char texto[32];
} Paquete;

uint32_t contador = 0;

void OnDataSent(const void *info, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "  -> entregado (ACK recibido)"
                                                : "  -> FALLO (nadie confirmo la recepcion)");
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
  memcpy(peer.peer_addr, macReceptor, 6);
  peer.channel = 0;
  peer.ifidx = WIFI_IF_STA;
  peer.encrypt = false;
  if (esp_now_add_peer(&peer) != ESP_OK) Serial.println("No se pudo agregar el peer");
  Serial.println("Emisor listo");
}

void loop() {
  Paquete p;
  p.contador = contador++;
  p.valor = random(200, 300) / 10.0f;          // valor simulado (ej. temperatura)
  strncpy(p.texto, "Hola desde el emisor", sizeof(p.texto));
  Serial.printf("Enviando #%lu valor=%.1f\n", (unsigned long)p.contador, p.valor);
  esp_now_send(macReceptor, (uint8_t *)&p, sizeof(p));
  delay(2000);
}
