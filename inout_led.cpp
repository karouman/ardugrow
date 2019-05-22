#include "inout_led.h"

Led::Led( int PIN ) {
  _PIN = PIN;
}

void Led::init() {
  pinMode( _PIN, OUTPUT );
}

void Led::on() {
  digitalWrite( _PIN, HIGH );
}

void Led::off() {
  digitalWrite( _PIN, LOW );
}

void Led::waitMillis( int mil ) {
  unsigned long lastmillis = millis();
  while( millis() - lastmillis < mil );
}

void Led::blink() {
  on();
  waitMillis( 50 );
  off();
  waitMillis( 50 );
  on();
  waitMillis( 50 );
  off();
  waitMillis( 50 );
  on();
  waitMillis( 150 );
  off();
}
