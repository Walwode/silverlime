#include <IRremoteESP8266.h>
#include <IRsend.h>
#include "Configuration.h"
#include "IrHandler.h"

IRsend *irsend = NULL;  // Set the GPIO to be used to sending the message.

void IrHandler::setup(byte pin) {
  Serial.println(F("Initialize infrared"));
  irsend = new IRsend(pin);
  irsend->begin();
}

void IrHandler::sendRaw(uint16_t rawData[]) {
  irsend->sendRaw(rawData, 67, 38);  // Send a raw data capture at 38kHz.  
}

void IrHandler::samsungPower() {
  Serial.println(F("IR Samsung Power"));
  uint16_t samsungPower[135] = IR_SAMSUNG_POWER;  // SAMSUNG E0E040BF
  sendRaw(samsungPower);
}

void IrHandler::samsungSmartHub() {
  Serial.println(F("IR Samsung Smart Hub"));
  uint16_t samsungSmartHub[135] = IR_SAMSUNG_SMARTHUB;  // SAMSUNG E0E09E61
  sendRaw(samsungSmartHub);
}

void IrHandler::samsungOk() {
  Serial.println(F("IR Samsung OK"));
  uint16_t samsungOk[135] = IR_SAMSUNG_OK;  // SAMSUNG E0E016E9
  sendRaw(samsungOk);
}

void IrHandler::receiverPower() {
  Serial.println(F("IR Receiver Power"));
  uint16_t receiverPower[21] = IR_RECEIVER_POWER;
  sendRaw(receiverPower);
}

void IrHandler::receiverPower1() {
  Serial.println(F("IR Receiver Power 1"));
  uint16_t receiverPower[21] = IR_RECEIVER_POWER_1;  // RC5 20C
  sendRaw(receiverPower);
}

void IrHandler::receiverPower2() {
  Serial.println(F("IR Receiver Power 2"));
  uint16_t receiverPower[21] = IR_RECEIVER_POWER_2;  // RC5 A0C
  sendRaw(receiverPower);
}


