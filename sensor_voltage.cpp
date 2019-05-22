#include "sensor_voltage.h"

Voltage::Voltage( int PIN ) {
  _PIN = PIN;
}

float Voltage::get() {
  int aVal = analogRead( _PIN );
  //return aVal * 1.0;
  // Get Battery state in Percent 
  // Tested for LIPO HV 5200mAh - 3S
  // 531 == 12.98 --> 100% ---> x: 0.02444
  // 506 == 12.48v --> 84% ---> x: 0.02466
  // 480 == 11.76v --> 3x% ---> x: 0.02450
  // 462 == 11.29v --> 10% ---> x: 0.02443
  return aVal * ( 0.0245075 );
}

int Voltage::getPercent() {
  float volt = get();
  if ( volt > 13.0 ) {
    volt = 13.0;
  }
  if ( volt < 11.29 ) {
    volt = 11.29;
  }
  return map( ( int )( volt * 100 ), 1129, 1300, 10, 100 );
}
