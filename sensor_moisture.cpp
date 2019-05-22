#include "sensor_moisture.h"

Moisture::Moisture( int PIN ) {
  _PIN = PIN;
}

int Moisture::get() {
  return analogRead( _PIN );
}

int Moisture::getAverage() {
  int val1 = get();
  waitMillis( 200 );
  int val2 = get();
  waitMillis( 200 );
  int val3 = get();
  waitMillis( 200 );
  int val4 = get();
  return ( int )( ( val1 + val2 + val3 + val4 ) / 4.0 );
}

void Moisture::waitMillis( int mil ) {
  unsigned long lastmillis = millis();
  while( millis() - lastmillis < mil );
}
