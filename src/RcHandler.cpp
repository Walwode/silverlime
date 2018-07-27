#include <RCSwitch.h>
#include "Configuration.h"
#include "RcHandler.h"

RCSwitch *rcSwitch = NULL;

void RcHandler::setup(byte sndPin, byte rcvPin, int protocol, int pulseLength) {
  Serial.print(F("Initialize 433MHz switches... "));
  rcSwitch = new RCSwitch();
  rcSwitch->enableTransmit(sndPin);
  rcSwitch->enableReceive(rcvPin);
  rcSwitch->setProtocol(protocol);
  rcSwitch->setPulseLength(pulseLength);
  Serial.println(F("done"));
}

void RcHandler::powerOnCh1() {
  Serial.println(F("RC Power On Channel 1"));
  rcSwitch->send(RC_POWER_ON_CH1);
}

void RcHandler::powerOffCh1() {
  Serial.println(F("RC Power Off Channel 1"));
  rcSwitch->send(RC_POWER_OFF_CH1);
}

void RcHandler::powerOnCh2() {
  Serial.println(F("RC Power On Channel 2"));
  rcSwitch->send(RC_POWER_ON_CH2);
}

void RcHandler::powerOffCh2() {
  Serial.println(F("RC Power Off Channel 2"));
  rcSwitch->send(RC_POWER_OFF_CH2);
}

void RcHandler::powerOnCh3() {
  Serial.println(F("RC Power On Channel 3"));
  rcSwitch->send(RC_POWER_ON_CH3);
}

void RcHandler::powerOffCh3() {
  Serial.println(F("RC Power Off Channel 3"));
  rcSwitch->send(RC_POWER_OFF_CH3);
}

void RcHandler::powerOnCh4() {
  Serial.println(F("RC Power On Channel 4"));
  rcSwitch->send(RC_POWER_ON_CH4);
}

void RcHandler::powerOffCh4() {
  Serial.println(F("RC Power Off Channel 4"));
  rcSwitch->send(RC_POWER_OFF_CH4);
}

void RcHandler::loop() {
  if (rcSwitch->available()) {
    Serial.println(F("[433MHz] >> Signal received:"));
    output(
      rcSwitch->getReceivedValue(),
      rcSwitch->getReceivedBitlength(),
      rcSwitch->getReceivedDelay(),
      rcSwitch->getReceivedRawdata(),
      rcSwitch->getReceivedProtocol());
    rcSwitch->resetAvailable();
    Serial.println(F("[433MHz] << Signal received"));
  }
}

void RcHandler::output(unsigned long decimal, unsigned int length, unsigned int delay, unsigned int* raw, unsigned int protocol) {

  const char* b = dec2binWzerofill(decimal, length);
  Serial.print(F("Decimal: "));
  Serial.print(decimal);
  Serial.print(F(" ("));
  Serial.print(length);
  Serial.print(F("Bit) Binary: "));
  Serial.print(b);
  Serial.print(F(" Tri-State: "));
  Serial.print(bin2tristate( b));
  Serial.print(F(" PulseLength: "));
  Serial.print(delay);
  Serial.print(F(" microseconds"));
  Serial.print(F(" Protocol: "));
  Serial.println(protocol);
  
  Serial.print(F("Raw data: "));
  for (unsigned int i=0; i<= length*2; i++) {
    Serial.print(raw[i]);
    Serial.print(F(","));
  }
  Serial.println();
}

char* RcHandler::bin2tristate(const char* bin) {
  static char returnValue[50];
  int pos = 0;
  int pos2 = 0;
  while (bin[pos]!='\0' && bin[pos+1]!='\0') {
    if (bin[pos]=='0' && bin[pos+1]=='0') {
      returnValue[pos2] = '0';
    } else if (bin[pos]=='1' && bin[pos+1]=='1') {
      returnValue[pos2] = '1';
    } else if (bin[pos]=='0' && bin[pos+1]=='1') {
      returnValue[pos2] = 'F';
    } else {
      return "not applicable";
    }
    pos = pos+2;
    pos2++;
  }
  returnValue[pos2] = '\0';
  return returnValue;
}

char* RcHandler::dec2binWzerofill(unsigned long Dec, unsigned int bitLength) {
  static char bin[64]; 
  unsigned int i=0;

  while (Dec > 0) {
    bin[32+i++] = ((Dec & 1) > 0) ? '1' : '0';
    Dec = Dec >> 1;
  }

  for (unsigned int j = 0; j< bitLength; j++) {
    if (j >= bitLength - i) {
      bin[j] = bin[ 31 + i - (j - (bitLength - i)) ];
    } else {
      bin[j] = '0';
    }
  }
  bin[bitLength] = '\0';
  
  return bin;
}

