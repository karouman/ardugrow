#ifndef WIFI_HEADER
#define WIFI_HEADER

#include <SoftwareSerial.h>
#include <Arduino.h>
#include "../config.h"

class Wifi {
    int _PIN_RX;
    int _PIN_TX;
    int _countTrueCommand;
    int _countTimeCommand;
    bool _found;
    SoftwareSerial* _wifi_serial;
  public:
    Wifi( int PIN_RX, int PIN_TX );
    bool connect( String SSID, String PASSWD );
    void disconnect();
    String postData( String host, String port, String uri, String data );
  private:
    void waitMillis( int mil );
    bool sendData( String command, const int timeout, char* readReplay );
};

#endif /* UNIQUE_ID_FOR_HEADER */
