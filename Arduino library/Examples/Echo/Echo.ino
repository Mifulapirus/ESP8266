/******************************************************************
* Description: ESP8266 Echo example
* This sample code helps connecting your arduino with the ESP8266 
* to your WiFi making it available from a regular TCP socket.
*
* YOU MUST SET THE SSID AND PASSWORD OF YOUR WIFI
//TODO: Right now it only works from Wifi to Arduino, it must go 
* the other way also
*******************************************************************
* Author = Angel Hernandez
* Contributors = Angel Hernandez
* License = GPL
* version = 0.1
* Contact = angel@tupperbot.com
*           @mifulapirus
*******************************************************************/
#include <SoftwareSerial.h>
#include <ESP8266.h>

#define wifiRx_pin 2
#define wifiTx_pin 3
#define wifiRST_pin 4

#define SSID "Your_Wifi_Name"
#define PASS "Your_Wifi_Pass"

ESP8266 wifi(wifiRx_pin, wifiTx_pin, wifiRST_pin, 9600);


void setup() {
  Serial.begin(9600);
  int Err = -1;
  String Ans="";
  
  Serial.print("Reboot --> ");
  Err = wifi.reboot();
  if (Err == NO_ERROR) {Serial.println("Reboot OK");}
  else {Serial.println(Err);}

  Serial.print("WiFi Mode --> ");
  Err = wifi.wifiMode(1);
  if (Err == NO_ERROR) {Serial.println("WiFi Mode OK");}
  else {Serial.println(Err);}
  
  Serial.print("Connect --> ");
  Err = wifi.connect(SSID, PASS);
  if (Err == NO_ERROR) {Serial.println("Connection OK");}
  else {
    Serial.println(Err);
    Serial.print(wifi.lastResponse);
  }
  
  Serial.print("Get IP --> ");
  String IP = wifi.getIP();
  Serial.println(IP);
  

  Serial.print("Connection MUX --> ");
  Err = wifi.connectionMode("1");
  if (Err == NO_ERROR) {Serial.println("Connection Mode OK");}
  else {Serial.println(Err);}
  
  Serial.print("ServerMode --> ");
  Err = wifi.setServer("5555");
  if (Err == NO_ERROR) {Serial.println("Server OK");}
  else {Serial.println(Err);}
}

void loop() {
  String cmd = wifi.readCmd();
  if (cmd!="") Serial.println(cmd);
}



  
