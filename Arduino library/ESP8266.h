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
#define ERROR_NOT_AT_OK						12

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
#define AT_CMD_CIPMODE_OFF		"AT+CIPMODE=0"
#define AT_CMD_CLOSE_CONNECTION	"AT+CIPCLOSE"
#define AT_CMD_SEND				"AT+CIPSEND="

//AT Responses
#define AT_RESP				"RCV: "
#define AT_RESP_READY		"Ready"
#define AT_RESP_IPD			"+IPD"
#define AT_RESP_LINK		"Link"
#define AT_RESP_UNLINK		"Unlink"
#define AT_RESP_OK     		"OK"
#define AT_RESP_SEND_OK		"SEND OK"
#define AT_RESP_NO_CHANGE	"no change"
#define AT_EMPTY_STRING		""
#define AT_TRUE				"1"
#define AT_FALSE			"0"

class ESP8266 : public SoftwareSerial {
  public:
    //Constructor
	ESP8266(byte _rxPin, byte _txPin, byte _rstPin, int _baud);
    
	//Public Functions
	byte checkBaudrate();
    byte init(String, String);
    byte reboot();
    byte reset();
    byte checkWifi();
	byte wifiMode(int);
	byte setServer(String);
	byte closeServer();
	byte connectionMode(String);
    byte connect(String _SSID, String _pass);
    String getIP();
    void setTxMode(boolean);
    byte expectResponse(char*);
	String getResponse(char*);
	boolean contains(String _s, String _search);
    byte openTCP(String, String, boolean _waitResponse = true);
    byte closeTCP();
    byte sendLongMessage(char*, boolean _waitResponse = true);
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
	String wifiLongMessage;

  private:
	//Private Functions
	
	//Private Variables
    byte rstPin;
	int _baudrates[13] = {300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 31250, 38400, 57600, 115200};
	int _baud; 
};

#endif
