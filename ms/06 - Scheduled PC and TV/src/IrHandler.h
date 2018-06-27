#ifndef IRHANDLER_H
#define IRHANDLER_H

#include <Arduino.h>

class IrHandler {
public:
  static void setup(byte pin); // ESP8266 GPIO pin to use. Recommended: 4 (D2).
  static void sendRaw(uint16_t rawData[]);

  static void samsungPower();
  static void samsungSmartHub();
  static void samsungOk();
  static void receiverPower1();
  static void receiverPower2();
};
#endif

