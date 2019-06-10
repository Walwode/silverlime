#ifndef RCHANDLER_H
#define RCHANDLER_H

#include <Arduino.h>

// 433MHz device
class RcHandler {
public:
  static void setup(byte sndPin, byte rcvPin, int protocol = 1, int pulseLength = 185); // pin 16 (D0)
  static void loop();
  static void output(unsigned long decimal, unsigned int length, unsigned int delay, unsigned int* raw, unsigned int protocol);
  static char* bin2tristate(const char* bin);
  static char* dec2binWzerofill(unsigned long Dec, unsigned int bitLength);

  static void powerOnCh1();
  static void powerOffCh1();
  static void powerOnCh2();
  static void powerOffCh2();
  static void powerOnCh3();
  static void powerOffCh3();
  static void powerOnCh4();
  static void powerOffCh4();
};
#endif

