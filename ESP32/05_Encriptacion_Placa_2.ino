/*
  PRACTICA 5 - ENCRIPTACION - PLACA DE TU COMPANERO
  Igual que la practica 4, pero los mensajes van cifrados (PMK + LMK).
  Ya trae la MAC de la otra placa (dc:da:0c:26:c3:74). No cambies nada.
  Las DOS placas deben tener EXACTAMENTE las mismas claves PMK_STR y LMK_STR (16 caracteres cada una).
  Experimento para la wiki: cambia un caracter de LMK_STR en UNA sola placa y los mensajes
  ya no llegan (normalmente sale "FALLO (sin ACK)").
*/
#include <WiFi.h>
#include <esp_now.h>

// >>> MAC de la OTRA placa <<<
uint8_t macOtraPlaca[6] = {0xDC, 0xDA, 0x0C, 0x26, 0xC3, 0x74};

// Claves: EXACTAMENTE 16 caracteres cada una (iguales en las dos placas)
const char PMK_STR[] = "PMK_clave_16byte";
const char LMK_STR[] = "LMK_clave_16byte";

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
  Serial.printf("<< [CIFRADO] Recibido #%lu \"%s\" (rssi=%d dBm)\n",
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

  // La PMK se configura ANTES de agregar peers
  esp_now_set_pmk((const uint8_t *)PMK_STR);

  esp_now_register_send_cb((esp_now_send_cb_t)OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, macOtraPlaca, 6);
  peer.channel = 0;
  peer.ifidx = WIFI_IF_STA;
  peer.encrypt = true;
  memcpy(peer.lmk, LMK_STR, 16);
  if (esp_now_add_peer(&peer) != ESP_OK) Serial.println("No se pudo agregar el peer cifrado");

  Serial.print("Mi MAC: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
  Paquete p;
  p.contador = contador++;
  snprintf(p.texto, sizeof(p.texto), "Secreto de %s", WiFi.macAddress().c_str());
  Serial.printf(">> Enviando cifrado #%lu\n", (unsigned long)p.contador);
  esp_now_send(macOtraPlaca, (uint8_t *)&p, sizeof(p));
  delay(3000);
}
