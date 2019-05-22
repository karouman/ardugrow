#ifndef LED_HEADER
#define LED_HEADER

#include <Arduino.h>

class Led {
    int _PIN;
  public:
    Led( int PIN );
    void init();
    void on();
    void off();
    void blink();
  private:
    void waitMillis( int mil );
};

#endif /* UNIQUE_ID_FOR_HEADER */
