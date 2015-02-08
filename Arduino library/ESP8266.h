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

//Error Table
  #define ErrorUnableToLink             1
  #define ErrorUnableToUnlink           2
  #define SendLongMessageError          3  
  #define ErrorModuleDoesntRespond      4
  #define ErrorModuleDoesntRespondToAT  5
  #define ErrorResponseNotFound         6
  #define ErrorNoResponse				7
  #define ErrorUnableToConnect          8
  #define ErrorStringNotFound			9
  
//AT Commands
#define AT_CMD_AT				"AT"
#define CIPSTART       			"AT+CIPSTART=\"TCP\",\""
#define AT_CMD_JOIN_AP			"AT+CWJAP=\""
#define AT_CMD_SERVER_MODE 		"AT+CIPSERVER="
#define AT_CMD_CONNECTION_MODE	"AT+CIPMUX="
#define AT_CMD_WIFI_MODE		"AT+CWMODE="
#define AT_CMD_GET_IP			"AT+CIFSR"
#define AT_CMD_CIPMODE_ON		"AT+CIPMODE=1"
#define AT_CMD_CIPMODE_OFF		"AT+CIPMODE=1"
#define AT_CMD_CLOSE_CONNECTION	"AT+CIPCLOSE"
#define AT_CMD_SEND				"AT+CIPSEND="

//AT Responses
#define AT_RESP				"RCV: "
#define AT_RESP_READY		"Ready"
#define AT_RESP_IPD			"+IPD"
#define AT_RESP_LINK		"Link"
#define AT_RESP_UNLINK		"Unlink"
#define AT_RESP_OK     		"OK"
#define AT_RESP_NO_CHANGE	"no change"


class ESP8266 : public SoftwareSerial {
  public:
    ESP8266(unsigned char RX_Pin, unsigned char TX_Pin, unsigned char RST_Pin);
    int CheckBaudrate();
    int InitWiFi(String SSID, String PASS);
    int WiFiReboot();
    int WiFiReset();
    int CheckWiFi();
	int WiFiMode(int);
	int ServerMode(String);
	int ConnectionMode(String);
    int ConnectWiFi(String SSID, String PASS);
    String GetIP();
    void SetCIPMODE(boolean);
    int ExpectResponse(char*);
	String GetResponse(char*);
	boolean Contains(String s, String search);
    int OpenTCP(String, String);
    int CloseTCP();
    int SendLongMessage(char*);
    char* WiFiRead();
	String ReadAll();
	String ReadCmd();
    void WiFiEcho();
    void SendDebug(String);
	
	String LastReceived;
	String IP;
	 

  private:
    int _RST_Pin;
	String _WiFiLongMessage;
};

#endif
