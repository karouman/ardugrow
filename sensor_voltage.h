#ifndef VOLTAGE_HEADER
#define VOLTAGE_HEADER
#include <Arduino.h>

class Voltage {
    int _PIN;
  public:
    Voltage( int PIN );
    float get();
    int getPercent();
};

#endif
