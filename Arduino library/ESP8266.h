/*
  ESP8266.h - Library for controlling the ESP8266 Wifi Module
  Created by Angel Hernandez, February 7, 2015.
  Released into the public domain.
*/

#ifndef ESP8266_h
#define ESP8266_h
#include "Arduino.h"
#include <SoftwareSerial.h>

//Error Table
#define NO_ERROR	0
#define ERROR_UNABLE_TO_LINK				1
#define ERROR_UNABLE_TO_UNLINK           	2
#define ERROR_SEND_LONG_MESSAGE          	3  
#define ERROR_MODULE_DOESNT_RESPOND			4
#define ERROR_MODULE_DOESNT_RESPOND_TO_AT 	5
#define ERROR_RESPONSE_NOT_FOUND         	6
#define ERROR_NO_RESPONSE					7
#define ERROR_UNABLE_TO_CONNECT          	8
#define ERROR_REBOOTING						9
#define ERROR_WIFI_MODE						10
#define ERROR_CONNECTION_MODE				11

//AT Commands
#define AT_CMD_AT				"AT"
#define AT_CMD_RST				"AT+RST"
#define AT_CMD_CIPSTART       	"AT+CIPSTART=\"TCP\",\""
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
    //Constructor
	ESP8266(unsigned char _rxPin, unsigned char _txPin, unsigned char _rstPin, int _baud);
    
	//Public Functions
	int checkBaudrate();
    int init(String, String);
    int reboot();
    int reset();
    int checkWifi();
	int wifiMode(int);
	int setServer(String);
	int connectionMode(String);
    int connect(String _SSID, String _pass);
    String getIP();
    void setTxMode(boolean);
    int expectResponse(char*);
	String getResponse(char*);
	boolean contains(String _s, String _search);
    int openTCP(String, String);
    int closeTCP();
    int sendLongMessage(char*);
    char* wifiRead();
	String readAll();
	String readCmd();
    void wifiEcho();
    void sendDebug(String);
	
	//Private variables
	String lastResponse;
	String IP;
	String serverPort;
	long responseTimeOut = 10000;
	 

  private:
	//Private Functions
	
	//Private Variables
    int rstPin;
	String wifiLongMessage;
	int _baudrates[13] = {300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 31250, 38400, 57600, 115200};
	int _baud; 
};

#endif
