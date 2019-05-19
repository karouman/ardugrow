#ifndef RELAY_HEADER
#define RELAY_HEADER

#include <Arduino.h>
#include "../config.h"

class Relay {
    int _PIN;
  public:
    Relay( int PIN );
    void on();
    void off();
	void onSeconds( int seconds );
};

#endif /* UNIQUE_ID_FOR_HEADER */
