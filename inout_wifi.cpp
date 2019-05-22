#include "inout_wifi.h"

Wifi::Wifi( int PIN_RX, int PIN_TX ) {
  _PIN_RX = PIN_RX;
  _PIN_TX = PIN_TX;

  _wifi_serial = new SoftwareSerial( _PIN_RX, _PIN_TX );

  _countTrueCommand = 0;
  _countTimeCommand = 0; 
  _found = false; 
}

bool Wifi::connect( String SSID, String PASSWD) {

  // Arduino does't work very well with ESP8266 at 115200 Baud rate 
  // First time connecting ESP8266, change the baud rate to 9600.
  // 1 - Connect ESP at 115200 then send AT+UART_DEF to change serial speed.
  // 2 - Next 'reboot', use 9600 Baud with SoftwareSerial and ESP8266
  //_wifi_serial->begin( 115200 ); // GPS; 9600 first time, 38400 second time
  //_wifi_serial->println( "AT+UART_DEF=9600,8,1,0,0" );

  _wifi_serial->begin( 9600 );
  sendData( "AT\r\n", 10, "OK" ); // Check Serial port / AT commands.
  sendData( "AT+RST\r\n", 10, "OK" ); // RESET ESP
  if ( sendData( "AT+CWMODE=1\r\n", 5, "OK" ) ) { // configure as access point and Client
    if ( sendData( "AT+CWJAP=\"" + SSID + "\",\"" + PASSWD + "\"\r\n", 20, "OK" ) ) { // Connect to AP
      return true;
    }
  }

  return false;
}

void Wifi::disconnect() {
  // TODO:
  // sendData( "AT+CWQAP\r\n", 5, "OK" ); // Quit AP
  // _countTrueCommand = 0;
  // _countTimeCommand = 0;
}

void Wifi::waitMillis( int mil ) {

  // delay() cause garbadge string when serial.read() 
  // the method implement trivila delay method

  unsigned long lastmillis = millis();
  while( millis() - lastmillis < mil );
}

String Wifi::postData( String host, String port, String uri, String data ) {
  // host: 192.168.43.239
  // port: 8092
  // uri: /upload
  // data: key=value&query=string&data=format
  String response = "";

  sendData( "AT+CIPMUX=0\r\n", 5, "OK" );
  //sendData( "AT+CIPSTART=\"TCP\",\"192.168.43.239\",8092\r\n", 20, "OK" );
  sendData( "AT+CIPSTART=\"TCP\",\"" + host + "\"," + port + "\r\n", 20, "OK" );

  String postRequest = "POST " + uri + " HTTP/1.1\r\n";
  postRequest = postRequest + "Host: " + host + ":" + port + "\r\n";
  postRequest = postRequest + "Accept: */*\r\n";
  postRequest = postRequest + "Content-Length: " + String( data.length() ) + "\r\n";
  postRequest = postRequest + "Connection: close\r\n";
  postRequest = postRequest + "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
  postRequest = postRequest + data;

  _wifi_serial->println( "AT+CIPSEND=" + String( postRequest.length() ) );
  waitMillis( 200 );
  if ( _wifi_serial->find( ">" ) ) {
    if ( _VERBOSE_ ) { Serial.println( "Sending data..." ); }
    _wifi_serial->print( postRequest );
    waitMillis( 200 );
    if ( _wifi_serial->find( "SEND OK" ) ) {
      while( _wifi_serial->available() ) {
        response = response + _wifi_serial->readString();
      }
      if ( _VERBOSE_ ) { Serial.println( response ); }
    }
  }
  _countTrueCommand++;

  return response;
}

bool Wifi::sendData( String command, const int timeout, char readReplay[] ) {
  bool found = false;

  if ( _VERBOSE_ ) {
    Serial.print( _countTrueCommand );
    //Serial.print( ":" );
    //Serial.print( _countTimeCommand );
    //Serial.print( ":" );
    //Serial.print( timeout );
    Serial.print( ". at command => " );
    Serial.print( command );
  }

  while( _countTimeCommand < ( timeout * 1 ) ) {
    if ( _VERBOSE_ ) { Serial.print( " *" ); }
    _wifi_serial->print( command );

    if( _wifi_serial->find( readReplay ) ) {
      found = true;
      break;
    }

    _countTimeCommand++;
  }

  if( found == true ) {
    if ( _VERBOSE_ ) { Serial.println( " OYI" ); }
    _countTrueCommand++;
    _countTimeCommand = 0;
  }

  if( found == false ) {
    if ( _VERBOSE_ ) { Serial.println( " FAIL" ); }
    _countTrueCommand = 0;
    _countTimeCommand = 0;
  }

  return found;
}
