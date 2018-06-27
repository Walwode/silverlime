#include <RCSwitch.h>
#include "RcHandler.h"

RCSwitch rcSwitch = RCSwitch();

void RcHandler::setup(byte pin) {
  Serial.println(F("Initialize 433MHz switches"));
  rcSwitch.enableTransmit(pin);
  rcSwitch.setProtocol(1);
  rcSwitch.setPulseLength(185);  
}

void RcHandler::powerOnF1() {
  Serial.println("RC Power On F1");
  rcSwitch.send("010001000001010100110011");
}

void RcHandler::powerOffF1() {
  Serial.println("RC Power Off F1");
  rcSwitch.send("010001000001010100111100");
}

void RcHandler::powerOnF2() {
  Serial.println("RC Power On F2");
  rcSwitch.send("010001000001010111000011");
}

void RcHandler::powerOffF2() {
  Serial.println("RC Power Off F2");
  rcSwitch.send("010001000001010111001100");
}

