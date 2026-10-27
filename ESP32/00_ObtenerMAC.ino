/*
  Muestra la direccion MAC (modo Station) de la placa.
  Cargalo en CADA placa y anota su MAC: la necesitaras en las practicas 2, 4 y 5.
*/
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  delay(2000);
  WiFi.mode(WIFI_STA);
  delay(100);
}

void loop() {
  uint8_t mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  Serial.printf("Para el codigo: {0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X}\n\n",
                mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  delay(3000);
}
