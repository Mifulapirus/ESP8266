/****************************************************
* Description: ESP8266 Arduino Library
* This library helps users working with the WiFi module ESP8266.
* It manages the AT command messages to communicate and configure 
* the module.
* It also manages the communication process by extracting the messages 
* sent through the serial port
*****************************************************
* Author__ = Angel Hernandez
* Contributors = Angel Hernandez
* License = GPL
* version = 0.2
* Contact = angel@tupperbot.com
* 			@mifulapirus
*****************************************************/
#include <SoftwareSerial.h>
#include "ESP8266.h"

// Contstructor
ESP8266::ESP8266(unsigned char _rxPin, unsigned char _txPin, unsigned char _rstPin, int _baud) : SoftwareSerial(_rxPin, _txPin) {
	begin(_baud);
	listen();
	rstPin = _rstPin;
	IP="0.0.0.0";
	pinMode(_rstPin, OUTPUT);
}
int ESP8266::checkBaudrate() {
  int _pause=500;
  
  for (int i=0; i<13; i++) {
    begin(_baudrates[i]);
    digitalWrite(rstPin, LOW);
    delay(_pause);
    digitalWrite(rstPin, HIGH);
    delay(_pause);
    wifiRead();
    delay(_pause);
  }
}
  
int ESP8266::init(String _SSID, String _pass) {
  if (reboot() != NO_ERROR) {return ERROR_REBOOTING;}
  if (wifiMode(1) != NO_ERROR)	{return ERROR_WIFI_MODE;}
  if (connect(_SSID, _pass) != NO_ERROR) {return ERROR_UNABLE_TO_CONNECT;}
  getIP();
  return NO_ERROR;
}

int ESP8266::reboot() {
  digitalWrite(rstPin, LOW);
  delay(500);
  digitalWrite(rstPin, HIGH);
  return expectResponse(AT_RESP_READY);
}

int ESP8266::reset() {
  flush();
  println(AT_CMD_RST); // restet and test if module is redy
  return expectResponse(AT_RESP_READY);
}

int ESP8266::checkWifi() {
  println(AT_CMD_AT);
  if (expectResponse(AT_RESP_OK)) {return NO_ERROR;}
  else {return ERROR_MODULE_DOESNT_RESPOND_TO_AT;}
}

int ESP8266::connect(String _SSID, String _pass) {
  String _cmd = AT_CMD_JOIN_AP;
  _cmd += _SSID;
  _cmd += "\",\"";
  _cmd += _pass;
  _cmd += "\"";
  println(_cmd);
  return expectResponse(AT_RESP_OK);
}
int ESP8266::setServer(String _port) {
	if (connectionMode("1") != NO_ERROR) {return ERROR_CONNECTION_MODE;}
	String _cmd = AT_CMD_SERVER_MODE;
	serverPort = _port;
	_cmd += "1,";
	_cmd += _port;
	println(_cmd);
	return expectResponse(AT_RESP_OK);
}


int ESP8266::connectionMode(String _mux) {
	String _cmd = AT_CMD_CONNECTION_MODE;
	_cmd += _mux;
	println(_cmd);
	return expectResponse(AT_RESP_OK);
}

int ESP8266::wifiMode(int _mode) {
	print(AT_CMD_WIFI_MODE);
	println(_mode);
	return expectResponse(AT_RESP_NO_CHANGE);
}

String ESP8266::getIP() {
	String _cmd = AT_CMD_GET_IP;
	String _msg="";
	println(_cmd); // Get IP
	_msg = readAll();
	IP = _msg.substring(11,_msg.length()-8);
	return IP;
}

void ESP8266::setCIPMODE(boolean _value) {
  if (_value) {println(AT_CMD_CIPMODE_ON);}
  else {println(AT_CMD_CIPMODE_OFF);}
}

int ESP8266::expectResponse(char* _expected) {
	String _received = "";
	for (int i = 0; i < 10000; i++) {
		_received = readAll();
		if (_received != "") {
			if (contains(_received, _expected))	return NO_ERROR;
		}
		delay(1);
	}
	if (_received == "") return ERROR_NO_RESPONSE;
	else return ERROR_RESPONSE_NOT_FOUND;
}
		

String ESP8266::readAll() {
  char _inChar;
  String _response = "";
  while (available()) {
    _inChar = read();
    _response += _inChar;
    delay(1);
  }
  lastResponse = _response;
  return _response;
}

String ESP8266::readCmd() {
	String _received = readAll();
	if (_received != "") {
		if (contains(_received, AT_RESP_NO_CHANGE)) 	{return AT_RESP_NO_CHANGE;}
		else if (contains(_received, AT_RESP_LINK)) 	{return AT_RESP_LINK;}
		else if (contains(_received, AT_RESP_UNLINK)) 	{return AT_RESP_UNLINK;}
		else if (contains(_received, AT_RESP_IPD)) 	{
			//FIND Channel, length and Msg
			String _channel = _received.substring(_received.indexOf(',')+1);
			String _length = _channel;
			_length = _length.substring(_channel.indexOf(',')+1, _channel.indexOf(':'));
			_channel = _channel.substring(0,_channel.indexOf(','));
			String _msg = _received.substring(_received.indexOf(':')+1, _received.indexOf(':')+1 + _length.toInt());
			return _msg;
		}
		
		else {
			String _unknown = "Unk: " + _received;
			return _unknown;
		}
	}
	return _received;
}

boolean ESP8266::contains(String _original, String _search) {
    int _searchLength = _search.length();
	int _max = _original.length() - _searchLength;
    for (int i = 0; i <= _max; i++) {
        if (_original.substring(i, i + _searchLength) == _search) {return true;}
    }
    return false;
} 

int ESP8266::openTCP(String _IP, String _port) {
  String _cmd = AT_CMD_CIPSTART; 
  _cmd += IP;
  _cmd += "\",";
  _cmd += _port;
  println(_cmd);
  if (!expectResponse(AT_RESP_LINK)) {
    return ERROR_UNABLE_TO_LINK;
  }
  return NO_ERROR;
}

int ESP8266::closeTCP() {
  println(AT_CMD_CLOSE_CONNECTION);
  if (!expectResponse(AT_RESP_UNLINK)) {
	return ERROR_UNABLE_TO_UNLINK;
	}
}

int ESP8266::sendLongMessage(char* _expected) {
  print(AT_CMD_SEND);
  println(wifiLongMessage.length());
  delay(200); 
  print(wifiLongMessage);
  if (expectResponse(_expected)) {return NO_ERROR;}
  else {return ERROR_SEND_LONG_MESSAGE;}
}

char* ESP8266::wifiRead() {
	char* _received;
	while (available()) {
		_received += read();
		delay(1);
    }
	return _received;
}  
