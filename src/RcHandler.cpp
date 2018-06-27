#include <RCSwitch.h>
#include "Configuration.h"
#include "RcHandler.h"

RCSwitch rcSwitch = RCSwitch();

void RcHandler::setup(byte pin, int protocol, int pulseLength) {
  Serial.println(F("Initialize 433MHz switches"));
  rcSwitch.enableTransmit(pin);
  rcSwitch.setProtocol(protocol);
  rcSwitch.setPulseLength(pulseLength);  
}

void RcHandler::powerOnF1() {
  Serial.println(F("RC Power On F1"));
  rcSwitch.send(RC_POWER_ON_F1);
}

void RcHandler::powerOffF1() {
  Serial.println(F("RC Power Off F1"));
  rcSwitch.send(RC_POWER_OFF_F1);
}

void RcHandler::powerOnF2() {
  Serial.println(F("RC Power On F2"));
  rcSwitch.send(RC_POWER_ON_F2);
}

void RcHandler::powerOffF2() {
  Serial.println(F("RC Power Off F2"));
  rcSwitch.send(RC_POWER_OFF_F2);
}

