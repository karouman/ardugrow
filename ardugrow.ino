#include "sensor_voltage.h"
#include "sensor_moisture.h"
#include "sensor_temperature.h"

#include "inout_relay.h"
#include "inout_switch.h"
#include "inout_led.h"
#include "inout_wifi.h"

#include "config.h"

// TEMPERATURE
#include <OneWire.h>

enum DS18B20_RCODES {
  READ_OK,
  NO_SENSOR_FOUND,  // Pas de capteur
  INVALID_ADDRESS,  // Adresse reçue invalide
  INVALID_SENSOR  // Capteur invalide (pas un DS18B20)
};

OneWire temperatureSensor( _PIN_TEMPERATURE_ );
// END TEMPERATURE

// extern "C" const String _PLANTS_[] = {  "Balcon", "Jasmin" };
// extern "C" const int _PIN_MOISTURE_[] = {     A5,      A5 };
// extern "C" const int _PIN_WATER_[] = {         3,       3 };

// extern "C" const String _PLANTS_[] = { "Marie" };
// extern "C" const int _PIN_MOISTURE_[] = {   A1 };
// extern "C" const int _PIN_WATER_[] = {       4 };

extern "C" const String _PLANTS_[] = { "Ficus" };
extern "C" const int _PIN_MOISTURE_[] = {   A5 };
extern "C" const int _PIN_WATER_[] = {       3 };

Voltage battery( _PIN_VOLTAGE_ );
Moisture moisture( _PIN_MOISTURE_[ 0 ] );
Wifi wifi( _PIN_WIFI_RX_, _PIN_WIFI_TX_ );
Relay waterPump( _PIN_WATER_[ 0 ] );
Led powerControl( _PIN_TIMER_ );
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
int parseResponse( String response ) {
    Serial.print( "Response: " );
    Serial.println( response );
    
    String data = response.substring( response.indexOf( '^' ) + 1 );
    data = data.substring( 0, data.indexOf( '^' ) );

    // Watter Pump Seconds ';' Next Recodes in X minutes
    int waterPumpSecondes = data.substring( 0, data.indexOf( ';' ) ).toInt();
    int nextRecordsMinutes = data.substring( data.indexOf( ';' ) + 1 ).toInt();

    Serial.print( "WaterPump Secondes" );
    Serial.println( waterPumpSecondes );

    return waterPumpSecondes;
    
    // run water pumb
}

void setup() {
    Serial.begin( 9600 );
    led.init();
    powerControl.init();

}

byte getTemperature(float *temperature, byte reset_search) {
  byte data[9], addr[8];
  // data[] : Données lues depuis le scratchpad
  // addr[] : Adresse du module 1-Wire détecté
  
  /* Reset le bus 1-Wire ci nécessaire (requis pour la lecture du premier capteur) */
  if (reset_search) {
    temperatureSensor.reset_search();
  }
 
  /* Recherche le prochain capteur 1-Wire disponible */
  if (!temperatureSensor.search(addr)) {
    // Pas de capteur
    return NO_SENSOR_FOUND;
  }
  
  /* Vérifie que l'adresse a été correctement reçue */
  if (OneWire::crc8(addr, 7) != addr[7]) {
    // Adresse invalide
    return INVALID_ADDRESS;
  }
 
  /* Vérifie qu'il s'agit bien d'un DS18B20 */
  if (addr[0] != 0x28) {
    // Mauvais type de capteur
    return INVALID_SENSOR;
  }
 
  /* Reset le bus 1-Wire et sélectionne le capteur */
  temperatureSensor.reset();
  temperatureSensor.select(addr);
  
  /* Lance une prise de mesure de température et attend la fin de la mesure */
  temperatureSensor.write(0x44, 1);
  delay(800);
  
  /* Reset le bus 1-Wire, sélectionne le capteur et envoie une demande de lecture du scratchpad */
  temperatureSensor.reset();
  temperatureSensor.select(addr);
  temperatureSensor.write(0xBE);
 
 /* Lecture du scratchpad */
  for (byte i = 0; i < 9; i++) {
    data[i] = temperatureSensor.read();
  }
   
  /* Calcul de la température en degré Celsius */
  *temperature = (int16_t) ((data[1] << 8) | data[0]) * 0.0625; 
  
  // Pas d'erreur
  return READ_OK;
}

int i = 0;
void loop() {

  float temp;
   
  /* Lit la température ambiante à ~1Hz */
  if (getTemperature(&temp, true) != READ_OK) {
    Serial.println(F("Erreur de lecture du capteur"));
    temp = -1;
  }

  for ( int i = 0; i < ( sizeof( _PLANTS_ ) / sizeof( _PLANTS_[ 0 ] ) ); i++ ) {
    delay(1000);
    String plantName = _PLANTS_[ i ];
    moisture = Moisture( _PIN_MOISTURE_[ i ] );
    waterPump = Relay( _PIN_WATER_[ i ] );
    waterPump.init();

    Serial.print( "Recording plant: " );
    Serial.println( plantName );
    String data =  "p=" + String( plantName );
    data = data + "&v=" + String( battery.get() );
    data = data + "&b=" + String( battery.getPercent() );
    data = data + "&m=" + String( moisture.getAverage() );
    data = data + "&t=" + String( temp );

    Serial.println( "Data = " + data );

    if ( wifi.connect( _WIFI_AP_SSID_, _WIFI_AP_PASSWD_ ) ) {
      String response = wifi.postData( _SERVER_HOST_, _SERVER_PORT_, _SERVER_URI_, data );
      led.blink();

      wifi.disconnect();

      int waterPumpSecondes = 0;
      waterPumpSecondes = parseResponse( response );

      if ( waterPumpSecondes > 0 ) {
        waterPump.on();
        delay( ( unsigned long )( 1000 * waterPumpSecondes ) );
        waterPump.off();
      }
    }
    
  }
  powerControl.on();
}
