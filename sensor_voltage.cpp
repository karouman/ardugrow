#include "sensor_voltage.h"

Voltage::Voltage( int PIN ) {
  _PIN = PIN;
}

float Voltage::get() {
  return analogRead( _PIN ) * ( 12.74 / 682 );
}

// volt*100 -> percent array
// 100%, 95%, 90%, 85%... 0%
const int tab[] = { 1290, 1281, 1260, 1245, 1230, 1215, 1200, 1182, 1176, 1161, 1155, 1146, 1134, 1128, 1119, 1113, 1107, 1101, 1098, 1062, 900 };

int Voltage::getPercent() {
  float volt = get();
  int voltI = ( int )( volt * 100.0 );

  int percent = 100;
  int i = 0;
  for ( ; i < 20; i++ ) {
    if ( voltI > tab[ i ] )
      break;
    percent -= 5;
  }


  return ( percent == 100 ? 100: percent + map( voltI, tab[ i ], tab[ i - 1 ], 0, 4 ) );
}
