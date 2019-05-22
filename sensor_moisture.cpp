#include "sensor_moisture.h"

Moisture::Moisture( int PIN ) {
  _PIN = PIN;
}

int Moisture::get() {
  return analogRead( _PIN );
}
