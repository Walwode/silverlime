#ifndef RCHANDLER_H
#define RCHANDLER_H

#include <Arduino.h>

// 433MHz device
class RcHandler {
public:
  static void setup(byte pin); // pin 16 (D0)

  static void powerOnF1();
  static void powerOffF1();
  static void powerOnF2();
  static void powerOffF2();
};
#endif

