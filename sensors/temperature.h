#ifndef TEMPERATURE_HEADER
#define TEMPERATURE_HEADER
#include <Arduino.h>

class Temperature {
    int _PIN;
  public:
    Temperature( int PIN );
    int getCelcius();
};

#endif
