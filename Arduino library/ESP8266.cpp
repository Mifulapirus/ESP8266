/*
  ESP8266.h - Library for controlling the ESP8266 Wifi Module
  Created by Angel Hernandez, February 7, 2015.
  Released into the public domain.
*/

#include <SoftwareSerial.h>
#include "ESP8266.h"


// Contstructor
ESP8266::ESP8266(unsigned char RX_Pin, unsigned char TX_Pin, unsigned char RST_Pin, long Baud) : SoftwareSerial(RX_Pin, TX_Pin) {
	begin(Baud);
	_RST_Pin = RST_Pin;
	IP="0.0.0.0";
	pinMode(_RST_Pin, OUTPUT);
}
int ESP8266::CheckBaudrate() {
  int Pause=500;
  long Baudrate[13] = {300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 31250, 38400, 57600, 115200};
  
  for (int i=0; i<13; i++) {
    begin(Baudrate[i]);
    digitalWrite(_RST_Pin, LOW);
    delay(Pause);
    digitalWrite(_RST_Pin, HIGH);
    delay(Pause);
    WiFiRead();
    delay(Pause);
  }
}
  
int ESP8266::InitWiFi(String SSID, String PASS) {
  if (WiFiReboot() != NO_ERROR) 			{return ErrorRebooting;}
  if (WiFiMode(1) != NO_ERROR)				{return ErrorWifiMode;}
  if (ConnectWiFi(SSID, PASS) != NO_ERROR) 	{return ErrorUnableToConnect;}
  GetIP();
  return NO_ERROR;
}

int ESP8266::WiFiReboot() {
  digitalWrite(_RST_Pin, LOW);
  delay(500);
  digitalWrite(_RST_Pin, HIGH);
  return ExpectResponse(AT_RESP_READY);
}

int ESP8266::WiFiReset() {
  flush();
  println("AT+RST"); // restet and test if module is redy
  return ExpectResponse(AT_RESP_READY);
}

int ESP8266::CheckWiFi() {
  SendDebug(AT_CMD_AT);
  if (ExpectResponse(AT_RESP_OK)) {
    return NO_ERROR;
  }
  else {
    return ErrorModuleDoesntRespondToAT;
  }
}

int ESP8266::ConnectWiFi(String SSID, String PASS) {
  String cmd = AT_CMD_JOIN_AP;
  cmd += SSID;
  cmd += "\",\"";
  cmd += PASS;
  cmd += "\"";
  println(cmd);
  return ExpectResponse(AT_RESP_OK);
}
int ESP8266::SetServer(String _Port) {
	if (ConnectionMode("1") != NO_ERROR) {return ErrorConnectionMode;}
	String cmd = AT_CMD_SERVER_MODE;
	ServerPort = _Port;
	cmd += "1,";
	cmd += _Port;
	println(cmd);
	return ExpectResponse(AT_RESP_OK);
}


int ESP8266::ConnectionMode(String _Mux) {
	String cmd = AT_CMD_CONNECTION_MODE;
	cmd += _Mux;
	println(cmd);
	return ExpectResponse(AT_RESP_OK);
}

int ESP8266::WiFiMode(int _Mode) {
	print(AT_CMD_WIFI_MODE);
	println(_Mode);
	return ExpectResponse(AT_RESP_NO_CHANGE);
}

String ESP8266::GetIP() {
	String _cmd = AT_CMD_GET_IP;
	String _msg="";
	println(_cmd); // Get IP
	_msg = ReadAll();
	IP = _msg.substring(11,_msg.length()-8);
	return IP;
}

void ESP8266::SetCIPMODE(boolean Value) {
  if (Value) {println(AT_CMD_CIPMODE_ON);}
  else {println(AT_CMD_CIPMODE_OFF);}
}

int ESP8266::ExpectResponse(char* _Expected) {
	String _received = "";
	for (int i = 0; i < 10000; i++) {
		_received = ReadAll();
		if (_received != "") {
			if (Contains(_received, _Expected))	return NO_ERROR;
		}
		delay(1);
	}
	if (_received == "") return ErrorNoResponse;
	else return ErrorResponseNotFound;
}
		

String ESP8266::ReadAll() {
  char _InChar;
  String _response = "";
  while (available()) {
    _InChar = read();
    _response += _InChar;
    delay(1);
  }
  LastReceived = _response;
  return _response;
}

String ESP8266::ReadCmd() {
	String _received = ReadAll();
	if (_received != "") {
		if (Contains(_received, AT_RESP_NO_CHANGE)) 	{return AT_RESP_NO_CHANGE;}
		else if (Contains(_received, AT_RESP_LINK)) 	{return AT_RESP_LINK;}
		else if (Contains(_received, AT_RESP_UNLINK)) 	{return AT_RESP_UNLINK;}
		else if (Contains(_received, AT_RESP_IPD)) 	{
			//FIND Channel, length and Msg
			String _channel = _received.substring(_received.indexOf(',')+1);
			String _length = _channel;
			_length = _length.substring(_channel.indexOf(',')+1, _channel.indexOf(':'));
			_channel = _channel.substring(0,_channel.indexOf(','));
			String _msg = _received.substring(_received.indexOf(':')+1, _received.indexOf(':')+1 + _length.toInt());
			return _msg;
		}
		
		else {
			String _Unknown = "Unknown: " + _received;
			return _Unknown;
			}
	}
	return _received;
}



boolean ESP8266::Contains(String original, String search) {
    int _searchLength = search.length();
	int _max = original.length() - _searchLength;
    for (int i = 0; i <= _max; i++) {
        if (original.substring(i, i+_searchLength) == search) {return true;}
    }
    return false;
} 

int ESP8266::OpenTCP(String IP, String Port) {
  String cmd = CIPSTART; 
  cmd += IP;
  cmd += "\",";
  cmd += Port;
  SendDebug(cmd);
  if (!ExpectResponse(AT_RESP_LINK)) {
    return ErrorUnableToLink;
  }
  return NO_ERROR;
}

int ESP8266::CloseTCP() {
  SendDebug(AT_CMD_CLOSE_CONNECTION);
  if (!ExpectResponse(AT_RESP_UNLINK)) {
	return ErrorUnableToUnlink;
	}
}

int ESP8266::SendLongMessage(char* ExpectedReply) {
  print(AT_CMD_SEND);
  println(_WiFiLongMessage.length());
  delay(200); 
  print(_WiFiLongMessage);
  if (ExpectResponse(ExpectedReply)) {
    return NO_ERROR;
    }
  else {
    return SendLongMessageError;
    }
}

char* ESP8266::WiFiRead() {
	char* _Received;
	while (available()) {
		_Received += read();
		delay(1);
    }
	return _Received;
}  

void ESP8266::SendDebug(String cmd) {
  println(cmd);
}

