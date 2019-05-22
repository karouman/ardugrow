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
Led led( 13 );

// TODO: add waitMillis to lib
void waitMillis( unsigned long mil ) {
  // delay() cause garbadge string when serial.read() 
  // the method implement trivila delay method

  unsigned long lastmillis = millis();
  while( millis() - lastmillis < mil );
}

//declare reset function @ address 0
void(* resetFunc) (void) = 0;

// extrac data from POST HTTP Response
// then run water pump
void parseResponse( String response ) {
    String data = response.substring( response.indexOf( '^' ) + 1 );
    data = data.substring( 0, data.indexOf( '^' ) );

    // Watter Pump Seconds ';' Next Recodes in X minutes
    int waterPumpSecondes = data.substring( 0, data.indexOf( ';' ) ).toInt();
    int nextRecordsMinutes = data.substring( data.indexOf( ';' ) + 1 ).toInt();

    // run water pumb
    if ( waterPumpSecondes > 0 ) {
      waterPump.on();
      waitMillis( ( unsigned long )( 1000 * waterPumpSecondes ) );
      waterPump.off();
    }

    waitMillis( ( unsigned long )( 60000 * nextRecordsMinutes ) );
}

void setup() {
    Serial.begin( 9600 );
    led.init();
    waterPump.init();
}

void loop() {
  waitMillis( 200 );

  String data =        "plant=" + String( _PLANT_NAME_ );
  data = data +        "&volt=" + String( battery.get() );
  data = data +        "&batt=" + String( battery.getPercent() );
  data = data +    "&moisture=" + String( moisture.getAverage() );
  data = data + "&temperature=" + String( temperature.getCelcius() );

  Serial.println( "Data = " + data );

  if ( wifi.connect( _WIFI_AP_SSID_, _WIFI_AP_PASSWD_ ) ) {
    String response = wifi.postData( _SERVER_HOST_, _SERVER_PORT_, _SERVER_URI_, data );
    led.blink();

    wifi.disconnect();

    parseResponse( response );

    resetFunc();
  }
  else {
    Serial.println( "Cannot connect wifi. Retry in 20 minutes" );
    waitMillis( ( unsigned long )( 1000 * 60 * 20 );
    resetFunc();
  }
}
