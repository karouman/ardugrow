#ifndef MOISTURE_HEADER
#define MOISTURE_HEADER
#include <Arduino.h>

class Moisture {
    int _PIN;
  public:
    Moisture( int PIN );
    int get();
    int getAverage();
  private:
    void waitMillis( int mil );
};

#endif
