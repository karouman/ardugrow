#include "sensor_temperature.h"

Temperature::Temperature( int PIN ) {
  _PIN = PIN;
}

int Temperature::readStableValue() {
  int diff = 5;
  int val1 = analogRead( _PIN );
  waitMillis( 200 );
  int val2 = analogRead( _PIN );
  if ( val1 > val2 ) {
    if ( val1 - val2 < diff )
      return val2;
  }
  else {
    if ( val2 - val1 < diff )
      return val2;
  }
  return readStableValue();
}


float Temperature::getCelcius() {
  //return analogRead( _PIN ) * 1.0;
  return readStableValue() * ( 5.0 / 1023.0 * 100.0 );
}

float Temperature::getFahrenheit() {
  return getCelcius() * 1.8 + 32;
}

void Temperature::waitMillis( int mil ) {
  unsigned long lastmillis = millis();
  while( millis() - lastmillis < mil );
}

