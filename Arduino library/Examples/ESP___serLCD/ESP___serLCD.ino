
#include <SoftwareSerial.h>
#include <ESP8266.h>
#include <serLCD.h>

#define display_pin A0

#define wifiRx_pin 2
#define wifiTx_pin 3
#define wifiRST_pin 4

#define FirmwareVersion  "0.1"
#define DeviceCategory   "UV Exposure Box" 
#define DeviceID         1 
#define DeviceName       "UV"

#define SSID "Interne"
#define PASS "Mec0mebien"
serLCD lcd(display_pin);
ESP8266 wifi(wifiRx_pin, wifiTx_pin, wifiRST_pin);



void setup() {
  wifi.listen();
  Serial.begin(9600);
  int Err = -1;
  lcd.clear();
  delay(2000);
  lcd.print("Rebooting Sheep"); 
  lcd.selectLine(2);
  lcd.print("WiFi module");
  Serial.print("Reboot --> ");
  Err = wifi.WiFiReboot();
  if (Err == NO_ERROR) {Serial.println("Reboot OK");}
  else {Serial.println(Err);}
  delay(1000);
  Serial.print("WiFi Mode --> ");
  Err = wifi.WiFiMode(1);
  if (Err == NO_ERROR) {Serial.println("WiFi Mode OK");}
  else {Serial.println(Err);}
  
  lcd.clear();
  lcd.print("Connecting Sheep");
  Serial.print("Connect --> ");
  Err = wifi.ConnectWiFi(SSID, PASS);
  if (Err == NO_ERROR) {Serial.println("Connection OK");}
  else {
    Serial.println(Err);
    Serial.print(wifi.LastReceived);
  }
  
  lcd.clear();
  lcd.print("Sheep's IP: ");
  lcd.selectLine(2);
  Serial.print("Get IP --> ");
  String IP = wifi.GetIP();
  lcd.print(IP);
  Serial.println(IP);
  

  Serial.print("Connection MUX --> ");
  Err = wifi.ConnectionMode("1");
  if (Err == NO_ERROR) {Serial.println("Connection Mode OK");}
  else {Serial.println(Err);}
  
  Serial.print("ServerMode --> ");
  Err = wifi.SetServer("5555");
  if (Err == NO_ERROR) {Serial.println("Server OK");}
  else {Serial.println(Err);}
}

void loop() {
  String cmd = wifi.ReadCmd();
  if (cmd!="") Serial.println(cmd);
  
  if (cmd == "ON") { 
    pinMode(A5, OUTPUT);
    digitalWrite(A5, HIGH);
  }
  
  else if (cmd == "OFF") {
    pinMode(A5, OUTPUT);
    digitalWrite(A5, LOW);
  }

  

}



  
