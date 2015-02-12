/******************************************************************
*** ATTENTION! YOU MUST INCLUDE SoftwareSerial.h IN YOUR SKETCH ***
*******************************************************************
* Description: ESP8266 Arduino Library
* This library helps users working with the WiFi module ESP8266.
* It manages the AT command messages to communicate and configure 
* the module.
* It also manages the communication process by extracting the messages 
* sent through the serial port
*******************************************************************
* Author = Angel Hernandez
* Contributors = Angel Hernandez
* License = GPL
* version = 0.2
* Contact = angel@tupperbot.com
* 			@mifulapirus
*******************************************************************/
#include <SoftwareSerial.h>
#include "ESP8266.h"

/*******************************************************************
* CONSTRUCTOR
* Sets some variables and opens the SoftwareSerial port leaving it
* listening. Not setting the listen() would make the module to not 
* see any command sent by the micro-controller
********************************************************************/
ESP8266::ESP8266(unsigned char _rxPin, unsigned char _txPin, unsigned char _rstPin, int _initBaud) : SoftwareSerial(_rxPin, _txPin) {
	_baud = _initBaud;
	//wifiLongMessage.reserve(400);
	begin(_baud);		//Open software serial port
	listen();			//Makes it the listening device
	rstPin = _rstPin;
	IP="0.0.0.0";
	pinMode(_rstPin, OUTPUT);
}

/*******************************************************************
* CHECK BAUDRATE
* Checks all available baudrates and exits when the ESP8266 answers 
* with the "Ready" command. 
********************************************************************/
int ESP8266::checkBaudrate() {
  int _pause=500;
  //Check baudrates and expects for the Ready command
  for (int i=0; i<13; i++) {
    _baud = _baudrates[i];
	begin(_baud);
    if (reboot() == NO_ERROR) return _baud;	//Returns the correct Ready command and leaves to port open
  }
}

/*******************************************************************
* INITIALIZATION
* This is the initialization routine for the module. In case of 
* error, it will exit with the proper error code (see table on 
* the ESP8266.h file)
********************************************************************/
int ESP8266::init(String _SSID, String _pass) {
  if (reboot() != NO_ERROR) {return ERROR_REBOOTING;}	
  if (wifiMode(1) != NO_ERROR)	{return ERROR_WIFI_MODE;} 
  if (connect(_SSID, _pass) != NO_ERROR) {return ERROR_UNABLE_TO_CONNECT;}
  getIP();
  return NO_ERROR;
}

/*******************************************************************
* REBOOT
* Hardware reset of the module. You must have connected the Reset 
* pin to some pin on your micro-controller and provide it as an 
* argument on the constructor
********************************************************************/
int ESP8266::reboot() {
  digitalWrite(rstPin, LOW);
  delay(500);
  digitalWrite(rstPin, HIGH);
  return expectResponse(AT_RESP_READY);
}

/*******************************************************************
* RESET
* Software reset of the module. this routine might not work if the 
* ESP8266 is not responding to any command, in which case, a reboot() 
* would be required
********************************************************************/
int ESP8266::reset() {
  flush();
  println(AT_CMD_RST); // restet and test if module is ready
  return expectResponse(AT_RESP_READY);
}

/*******************************************************************
* CHECK WIFI
* Simply sends the "AT" command and expects the "OK" response in 
* in order to make sure the module is working fine.
********************************************************************/
int ESP8266::checkWifi() {
  println(AT_CMD_AT);
  if (expectResponse(AT_RESP_OK)) {return NO_ERROR;}
  else {return ERROR_MODULE_DOESNT_RESPOND_TO_AT;}
}

/*******************************************************************
* CONNECT TO WIFI NETWORK
* This function builds the connection string and waits for the 
* module to be connected.
* You must provide the correct WiFi SSID (Name) and Password
********************************************************************/
int ESP8266::connect(String _SSID, String _pass) {
  String _cmd = AT_CMD_JOIN_AP;
  _cmd += _SSID;
  _cmd += "\",\"";
  _cmd += _pass;
  _cmd += "\"";
  println(_cmd);
  return expectResponse(AT_RESP_OK);
}

/*******************************************************************
* SET THE SERVER MODE
* Configures the module in Server mode on the given port. From that 
* moment on, it will be possible to talk to the module on that port
* by opening a TCP connection on its IP (You can use Putty to do so)
********************************************************************/
int ESP8266::setServer(String _port) {
	if (connectionMode("1") != NO_ERROR) {return ERROR_CONNECTION_MODE;}
	String _cmd = AT_CMD_SERVER_MODE;
	serverPort = _port;
	_cmd += "1,";
	_cmd += _port;
	println(_cmd);
	return expectResponse(AT_RESP_OK);
}

/*******************************************************************
* CONNECTION MODE
********************************************************************/
int ESP8266::connectionMode(String _mux) {
	String _cmd = AT_CMD_CONNECTION_MODE;
	_cmd += _mux;
	println(_cmd);
	return expectResponse(AT_RESP_OK);
}

/*******************************************************************
* WIFI MODE: AP or SAT
* The ESP8266 can be set as an Access Point (AP, _mode = 1) or as 
* a Station (STA, _mode=2) connected to the given WiFi (see connect())
* Most of the time you will use it just in STA mode so it connects to 
* your WiFi as any other device.
********************************************************************/
int ESP8266::wifiMode(int _mode) {
	print(AT_CMD_WIFI_MODE);
	println(_mode);
	return expectResponse(AT_RESP_NO_CHANGE);
}

/*******************************************************************
* GET IP
* Returns the given IP and sets the public IP variable.
********************************************************************/
String ESP8266::getIP() {
	//TODO: Check if the module is actually connected
	String _cmd = AT_CMD_GET_IP;
	String _msg="";
	println(_cmd); // Get IP
	_msg = readAll();
	IP = _msg.substring(11,_msg.length()-8);
	return IP;
}

/*******************************************************************
* SET TRANSMISSION MODE
* Sets the transmission mode to NORMAL or TRANSPARENT
//TODO: Explain the difference and talk about the +IPD
********************************************************************/
void ESP8266::setTxMode(boolean _value) {
  if (_value) {println(AT_CMD_CIPMODE_ON);}
  else {println(AT_CMD_CIPMODE_OFF);}
}

/*******************************************************************
* EXPECT A GIVEN RESPONSE
* Waits for the module to provide the given response. If there is no 
* answer or the answer does not contain the given String in the 
* specified responseTimeOut, the function will return with a 
* ERROR_NO_RESPONSE or ERROR_RESPONSE_NOT_FOUND
********************************************************************/
int ESP8266::expectResponse(char* _expected) {
	String _received = "";
	for (int i = 0; i < responseTimeOut; i++) {
		_received = readAll();
		if (_received != "") {
			if (contains(_received, _expected))	return NO_ERROR;
		}
		delay(1);
	}
	if (_received == "") return ERROR_NO_RESPONSE;
	else return ERROR_RESPONSE_NOT_FOUND;
}
		
/*******************************************************************
* READS ALL CHARACTERS FROM BUFFER
* Reads all characters sent BY the ESP8266 TO the micro-controller
* saves the response as the lastResponse, and returns with the String
********************************************************************/
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

/*******************************************************************
* READS A SPECIFIC COMMAND
* This function reacts to the provided AT commands, including the 
* +IPD extracting the characters from the message being received and 
* obtaining the current channel and length
//TODO: Do a better job at explaining this function. It's just too late.
//TODO: consider dividing the AT_RESP_IPD into an isolated function
********************************************************************/
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

/*******************************************************************
* CONTAINS A GIVEN STRING
* Checks if the "Original" string contains the "Search" string and 
* returns true or false.
********************************************************************/
boolean ESP8266::contains(String _original, String _search) {
    int _searchLength = _search.length();
	int _max = _original.length() - _searchLength;
    for (int i = 0; i <= _max; i++) {
        if (_original.substring(i, i + _searchLength) == _search) {return true;}
    }
    return false;
} 

/*******************************************************************
* OPEN TCP CONNECTION
* Opens a TCP connection to the given server on the _IP and _port 
* provided. 
//TODO: Implement the send function
********************************************************************/
int ESP8266::openTCP(String _IP, String _port) {
  String _cmd = AT_CMD_CIPSTART; 
  _cmd += _IP;
  _cmd += "\",";
  _cmd += _port;
  println(_cmd);
  if (expectResponse(AT_RESP_LINK) != NO_ERROR) {return ERROR_UNABLE_TO_LINK;}
  return NO_ERROR;
}

/*******************************************************************
* CLOSE TCP CONNECTION
* Closes a TCP connection.
********************************************************************/
int ESP8266::closeTCP() {
  println(AT_CMD_CLOSE_CONNECTION);
  if (expectResponse(AT_RESP_UNLINK) != NO_ERROR) {return ERROR_UNABLE_TO_UNLINK;}
  return NO_ERROR;
}

/*******************************************************************
* SEND LONG MESSAGE 
* Sends a long message provided on the WifiLongMessage variable
//TODO: This needs a lot of improvement, it is only useful to 
* abstract the user from the pointer
********************************************************************/
int ESP8266::sendLongMessage(char* _expected) {
  print(AT_CMD_SEND);
  println(wifiLongMessage.length());
  delay(200); 
  print(wifiLongMessage);
  if (expectResponse(_expected) == NO_ERROR) {return NO_ERROR;}
  else {return ERROR_SEND_LONG_MESSAGE;}
}
