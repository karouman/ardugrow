#include "inout_relay.h"

Relay::Relay( int PIN ) {
  _PIN = PIN;
}

void Relay::init() {
  pinMode( _PIN, OUTPUT );
  digitalWrite( _PIN, LOW );
}

void Relay::on() {
	// TODO
  Serial.println( "RELAY - ON" );
  digitalWrite( _PIN, HIGH );
}

void Relay::off() {
	// TODO
  Serial.println( "RELAY - OFF" );
  digitalWrite( _PIN, LOW );
}

void Relay::onSeconds( int seconds ) {
	// TODO
}
