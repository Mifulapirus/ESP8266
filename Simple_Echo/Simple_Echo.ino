#include <SoftwareSerial.h>

#define WIFI_RX_PIN 2
#define WIFI_TX_PIN 3
#define WIFI_RST_PIN 4
#define BAUD 115200

SoftwareSerial wifi(WIFI_RX_PIN, WIFI_TX_PIN);

void setup() {
  wifi.begin(9600);
  Serial.begin(9600);
}

void loop() {
  if (wifi.available()) {Serial.write(wifi.read());}
  if (Serial.available()) {wifi.write(Serial.read());}
}
