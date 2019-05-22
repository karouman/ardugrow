#include "inout_relay.h"

Relay::Relay( int PIN ) {
  _PIN = PIN;
}

void Relay::init() {
  pinMode( _PIN, OUTPUT );
  off();
}

void Relay::on() {
	// TODO
  Serial.println( "RELAY - ON" );
  digitalWrite( _PIN, LOW );
}

void Relay::off() {
	// TODO
  Serial.println( "RELAY - OFF" );
  digitalWrite( _PIN, HIGH );
}

void Relay::onSeconds( int seconds ) {
	// TODO
}
