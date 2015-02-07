/*
  ESP8266.h - Library for controlling the ESP8266 Wifi Module
  Created by Angel Hernandez, February 7, 2015.
  Released into the public domain.
*/

#include "SoftwareSerial.h"
#include "ESP8266.h"


// Contstructor
ESP8266::ESP8266(unsigned char RX_Pin, unsigned char TX_Pin, unsigned char RST_Pin) : SoftwareSerial(RX_Pin, TX_Pin) {
	begin(9600);
	_RST_Pin = RST_Pin;
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
  while (!WiFiReboot())  {}
  WiFiMode(1);
  //while (!CheckWiFi())   {}
  while (!ConnectWiFi(SSID, PASS)) {}
  SetCIPMODE(false);
  return NO_ERROR;
}

int ESP8266::WiFiReboot() {
  digitalWrite(_RST_Pin, LOW);
  delay(500);
  digitalWrite(_RST_Pin, HIGH);
  return ExpectResponse("Ready");
}

int ESP8266::WiFiReset() {
  flush();
  println("AT+RST"); // restet and test if module is redy
  return ExpectResponse("Ready");
}

int ESP8266::CheckWiFi() {
  SendDebug("AT");
  if (ExpectResponse(ResponseOK)) {
    return NO_ERROR;
  }
  else {
    return ErrorModuleDoesntRespondToAT;
  }
}

int ESP8266::ConnectWiFi(String SSID, String PASS) {
  String cmd = "AT+CWJAP=\"";
  cmd += SSID;
  cmd += "\",\"";
  cmd += PASS;
  cmd += "\"";
  println(cmd);

  String OwnIP = "";
  char incomingByte = ' ';
  for(int i=0; i<cmd.length()+8 ;i++) read();
  
  if (ExpectResponse(ResponseOK)) {
    return NO_ERROR;
  }
  else {
    return ErrorUnableToConnect;
  }
}

int ESP8266::WiFiMode(int _Mode) {
	print("AT+CWMODE=");
	println(_Mode);
	return ExpectResponse("no change");
}

String ESP8266::GetIP() {
	String cmd = "AT+CIFSR";
	String OwnIP = "";
	char incomingByte = ' ';
	println(cmd); // Get IP
	for(int i=0; i<cmd.length()+5 ;i++) read();
	delay(100);
	while (available() > 0) {
		// read the incoming byte:
		incomingByte = read();
		// say what you got:
		OwnIP += incomingByte;
		delay(1);
	}
	return OwnIP;
}

void ESP8266::SetCIPMODE(boolean Value) {
  if (Value) {println("AT+CIPMODE=1");}
  else {println("AT+CIPMODE=0");}
}

int ESP8266::ExpectResponse(char* _Expected) {
  for (int i = 0; i < 10; i++) {
    if (find(_Expected)) {
		while (available()) {read();}
		return NO_ERROR;
    }

    else {
      //TODO: Do something if not found yet?
    }
    delay(1);
  }
  return ErrorResponseNotFound;
}

String ESP8266::GetResponse(char* Expected) {
	long _timeout = 10000;
	//String _Expected = Expected;
	String _response;
  
	while (_timeout > 0) {
		_timeout--;
		while (available()) {
			_response += String(read());
			delay(1);
		}
		if (Contains(Expected, _response) == NO_ERROR) {return "found";}
		delay(1);
	}
	return _response;
}


int ESP8266::Contains(String s, String search) {
    int max = s.length() - search.length();
    for (int i = 0; i <= max; i++) {
        if (s.substring(i) == search) return NO_ERROR; 
    }
    return ErrorStringNotFound;
} 

int ESP8266::OpenTCP(String IP, String Port) {
  String cmd = CIPSTART; 
  cmd += IP;
  cmd += "\",";
  cmd += Port;
  SendDebug(cmd);
  if (!ExpectResponse("Linked")) {
    return ErrorUnableToLink;
  }
  return NO_ERROR;
}

int ESP8266::CloseTCP() {
  SendDebug("AT+CIPCLOSE");
  if (!ExpectResponse("Unlink")) {
	return ErrorUnableToUnlink;
	}
}

int ESP8266::SendLongMessage(char* ExpectedReply) {
  print("AT+CIPSEND=");
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

