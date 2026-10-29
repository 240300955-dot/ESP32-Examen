# Interfaces de Hardware - Proyecto 2do parcial ESP32-S3

Prácticas de comunicación inalámbrica con **ESP-NOW** usando dos placas **ESP32-S3**, desarrolladas en equipo por:

- Derek Monter
- Renata Valle

ESP-NOW permite que dos placas ESP32 se comuniquen directamente, sin router ni red WiFi, identificándose por su dirección MAC.

## Requisitos

- Arduino IDE 2.x
- Soporte para placas ESP32 instalado en el Gestor de placas
- Placa seleccionada: **ESP32S3 Dev Module**
- Monitor serie a **115200 baudios**
- Dos placas ESP32-S3 (una por persona)

## Prácticas

| Práctica | Descripción | Archivos |
|---|---|---|
| 00 | Obtener la dirección MAC de cada placa | `00_ObtenerMAC.ino` |
| 01 | Distancia máxima: RSSI y paquetes perdidos | `01_Distancia_Emisor.ino`, `01_Distancia_Receptor.ino` |
| 02 | Comunicación unidireccional con confirmación (ACK) | `02_Unidireccional_Emisor.ino`, `02_Unidireccional_Receptor.ino` |
| 03 | Broadcast: un mensaje para todas las placas cercanas | `03_Broadcast_Emisor.ino`, `03_Broadcast_Receptor.ino` |
| 04 | Comunicación bidireccional | `04_Bidireccional_Placa_1.ino`, `04_Bidireccional_Placa_2.ino` |
| 05 | Comunicación bidireccional con cifrado | `05_Encriptacion_Placa_1.ino`, `05_Encriptacion_Placa_2.ino` |

## Cómo usarlo

1. Carga `00_ObtenerMAC.ino` en cada placa y anota su dirección MAC.
2. En las prácticas 02, 04 y 05, cambia la línea de la MAC (`macReceptor` o `macOtraPlaca`) por la MAC de la otra placa.
3. Carga cada sketch en su placa correspondiente (emisor/receptor o Placa 1/Placa 2).
4. Abre el monitor serie a 115200 baudios para ver los resultados.

## Autores

Derek Monter y Renata Valle
