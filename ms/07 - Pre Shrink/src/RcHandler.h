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

  static void powerOnF1();
  static void powerOffF1();
  static void powerOnF2();
  static void powerOffF2();
};
#endif

