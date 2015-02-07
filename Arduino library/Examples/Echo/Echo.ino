#include <ESP8266.h>

#define wifiRx_pin 2
#define wifiTx_pin 3
#define wifiRST_pin 4

ESP8266 wifi(wifiRx_pin, wifiTx_pin, wifiRST_pin);

void setup() {
  Serial.begin(9600);
  int Err = -1;
  String Ans="";
  
  Serial.print("Reboot --> ");
  Err = wifi.WiFiReboot();
  if (Err == NO_ERROR) {Serial.println("Reboot OK");}
  else {Serial.println(Err);}

  Serial.print("WiFi Mode --> ");
  Err = wifi.WiFiMode(1);
  if (Err == NO_ERROR) {Serial.println("WiFi Mode OK");}
  else {Serial.println(Err);}
  
  Serial.print("Connect --> ");
  Err = wifi.ConnectWiFi("Interne", "Mec0mebien");
  if (Err == NO_ERROR) {Serial.println("Connection OK");}
  else {Serial.println(Err);}
  
  Serial.print("Get IP --> ");
  String IP = wifi.GetIP();
  Serial.println(IP);

}

void loop() {
  // put your main code here, to run repeatedly:

}