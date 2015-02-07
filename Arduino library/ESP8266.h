/*
  ESP8266.h - Library for controlling the ESP8266 Wifi Module
  Created by Angel Hernandez, February 7, 2015.
  Released into the public domain.
*/


#ifndef ESP8266_h
#define ESP8266_h
#include "Arduino.h"
#include "SoftwareSerial.h"

//Messages sent
#define NO_ERROR	0
#define ResponseOK     "OK"
#define ResponseCheckDisabled	-1
#define Error          "ERROR "
  #define ErrorUnableToLink             1
  #define ErrorUnableToUnlink           2
  #define SendLongMessageError          3  
  #define ErrorModuleDoesntRespond      4
  #define ErrorModuleDoesntRespondToAT  5
  #define ErrorResponseNotFound         6
  #define ErrorUnableToConnect          7
  #define ErrorStringNotFound			8
  
#define CIPSTART       "AT+CIPSTART=\"TCP\",\""

class ESP8266 : public SoftwareSerial {
  public:
    ESP8266(unsigned char RX_Pin, unsigned char TX_Pin, unsigned char RST_Pin);
    int CheckBaudrate();
    int InitWiFi(String SSID, String PASS);
    int WiFiReboot();
    int WiFiReset();
    int CheckWiFi();
	int WiFiMode(int);
    int ConnectWiFi(String SSID, String PASS);
    String GetIP();
    void SetCIPMODE(boolean);
    int ExpectResponse(char*);
	String GetResponse(char*);
	int Contains(String s, String search);
    int OpenTCP(String, String);
    int CloseTCP();
    int SendLongMessage(char*);
    char* WiFiRead();
    void WiFiEcho();
    void SendDebug(String);

  private:
    int _RST_Pin;
	String _WiFiLongMessage;
};

#endif
