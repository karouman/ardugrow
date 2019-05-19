#include "./sensors/voltage.h"
#include "./sensors/moisture.h"
#include "./sensors/temperature.h"

#include "./inout/relay.h"
#include "./inout/switch.h"
#include "./inout/wifi.h"

#include "./config.h"

// Sensors
Voltage battery( A0 );
Moisture moisture( A1 );
Temperature temperature( A6 );

// in / out
Wifi wifi( A5, A4);
Relay waterPump( A5 );
Switch sendDataSwich( A7 );

// extrac data from POST HTTP Response
void parseResponse( String response, int &water, int &mill ) {
    // TODO
    Serial.print( response );
    water = 0;
    mill = 20000;
}

void setup()
{
    Serial.begin( 9600 );
}

int lastmillis = 0;

void loop()
{
    if (millis() - lastmillis >= 1000) {

        // if ( wifi.connect( "HUAWE-E5186-41D5", "3461H94B89" ) ) {
        String data =   "batt=" + String( battery.getPercent() );
        data = data + "&moise=" + String( moisture.get() );
        data = data + "&temp=" + String( temperature.getCelcius() );

        if ( wifi.connect( _WIFI_AP_SSID_, _WIFI_AP_PASSWD_ ) ) {
            wifi.disconnect();

            String response = wifi.postData( _SERVER_HOST_, _SERVER_PORT_, _SERVER_URI_, data );

            int water = 0; // Need Water for n Seconds
            int mill = 20000; // Next SendData is required in n
            parseResponse( response, water, mill );

            if ( water > 0 ) {
                waterPump.onSeconds( water );
            }

        }
        lastmillis = millis();
    }
}
