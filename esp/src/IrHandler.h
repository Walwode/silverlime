#ifndef IRHANDLER_H
#define IRHANDLER_H

#include <Arduino.h>

class IrHandler {
public:
  IrHandler() {};
  ~IrHandler() {};

  static void setup(byte sndPin, byte rcvPin);
  static void loop();
  static void sendRaw(uint16_t rawData[]);

  static void samsungPower();
  static void samsungSmartHub();
  static void samsungOk();
  static void receiverPower();
};

#endif

