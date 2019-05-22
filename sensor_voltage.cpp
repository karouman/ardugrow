#include "sensor_voltage.h"

// Get Battery state in Percent 
// Tested for LIPO HV 5200mAh - 3S
// 11.0v -> 6% CRITICAL --> analog:478
// 12.8V ==> 100% --> analog:561
// 11.5V ==> xx% --> analog:500
// 10.5V ==> --% --> analog:457

Voltage::Voltage( int PIN ) {
  _PIN = PIN;
}

int Voltage::getPercent() {
  int aVal = analogRead( _PIN );

  int aMin = 485; //478;
  int aMax = 561 - aMin;
  aVal = aVal - aMin;
  
  if ( aVal < 0 )
    return -1;

  int p = ( ( aVal *  100 ) / aMax );

  if ( p < 0 )
    return -1;
  if ( p > 100 )
    return 100;

  return p;
}
