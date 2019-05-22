#ifndef VOLTAGE_HEADER
#define VOLTAGE_HEADER
#include <Arduino.h>

class Voltage {
    int _PIN;
  public:
    Voltage( int PIN );
    int getPercent();
};

#endif
