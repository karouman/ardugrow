#include "sensor_voltage.h"
#include "sensor_moisture.h"
#include "sensor_temperature.h"

#include "inout_relay.h"
#include "inout_switch.h"
#include "inout_led.h"
#include "inout_wifi.h"

#include "config.h"

Voltage battery( A0 );
Moisture moisture( A1 );
Wifi wifi( A3, A4);
Relay waterPump( A5 );
Temperature temperature( A6 );
Relay powerLine( A7 );
Led led( 13 );

void waitMillis( unsigned long mil ) {
  // delay() cause garbadge string when serial.read() 
  // the method implement trivila delay method

  // Serial.print( 'Wait for mills' );
  // Serial.println( mil );
  unsigned long lastmillis = millis();
  while( millis() - lastmillis < mil );
}

void(* resetFunc) (void) = 0; //declare reset function @ address 0

// extrac data from POST HTTP Response
void parseResponse( String response, int &water, int &mill ) {
    // TODO
    String data = response.substring( response.indexOf( '^' ) + 1 );
    data = data.substring( 0, data.indexOf( '^' ) );

	// Watter Pump Seconds ';' Next Recodes in X minutes
    int waterPumpSecondes = data.substring( 0, data.indexOf( ';' ) ).toInt();
    int nextRecordsMinutes = data.substring( data.indexOf( ';' ) + 1 ).toInt();

    // Serial.print( "$$" );
    // Serial.print( waterPump );
    // Serial.print( ',' );
    // Serial.print( nextRecords );
    // Serial.println( "$$" );

   // if ( waterPumpSecondes > 0 ) {
   //   waterPump.on();
   //   waitMillis( ( unsigned long )( waterPumpSecondes * 1000 ) );
   //   waterPump.off();
   // }
    waitMillis( ( unsigned long )( 60000 * nextRecordsMinutes ) );
}

void setup()
{
    Serial.begin( 9600 );
    Serial.println( "SETUP" );
    led.init();
    waterPump.init();
    powerLine.init();
}


void loop()
{
  waitMillis( 200 );

  powerLine.on();
  waitMillis( 200 );

  String data =      "battery=" + String( battery.getPercent() );
  data = data +    "&moisture=" + String( moisture.get() );
  data = data + "&temperature=" + String( temperature.getCelcius() );
  Serial.println( "Data = " + data );

  if ( wifi.connect( _WIFI_AP_SSID_, _WIFI_AP_PASSWD_ ) ) {
    wifi.disconnect();

    String response = wifi.postData( _SERVER_HOST_, _SERVER_PORT_, _SERVER_URI_, data );

    int water = 0; // Need Water for n Seconds
    int mill = 20000; // Next SendData is required in n
    parseResponse( response, water, mill );

    powerLine.off();
    resetFunc();
    waitMillis( 3000 );
  }
  else {
    Serial.println( "Cannot connect wifi..." );
  }
  led.blink();
  waitMillis( 20000 );
}
