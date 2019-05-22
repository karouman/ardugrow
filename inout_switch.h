#ifndef SWITCH_HEADER
#define SWITCH_HEADER

#include <Arduino.h>
#include "config.h"

class Switch {
    int _PIN;
  public:
    Switch( int PIN );
};

#endif
