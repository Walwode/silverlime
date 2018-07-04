#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRutils.h>
#include "Configuration.h"
#include "IrHandler.h"

#define CAPTURE_BUFFER_SIZE 1024

IRsend *irsend = NULL; // Set the GPIO to be used to sending the message
IRrecv *irrecv = NULL; // set the GPIO to be used to reveice the message
decode_results results;  // Somewhere to store the results

void IrHandler::setup(byte sndPin, byte rcvPin) {
  Serial.println(F("Initialize infrared"));
  irsend = new IRsend(sndPin);
  irsend->begin();

  irrecv = new IRrecv(rcvPin, CAPTURE_BUFFER_SIZE, 15U, true);
#if DECODE_HASH
  irrecv->setUnknownThreshold(12); // Ignore messages with less than minimum on or off pulses.
#endif  // DECODE_HASH
  irrecv->enableIRIn();  // Start the receiver
}

void IrHandler::sendRaw(uint16_t rawData[]) {
  irsend->sendRaw(rawData, 67, 38);  // Send a raw data capture at 38kHz.  
  yield();
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
  uint16_t receiverPower1[21] = IR_RECEIVER_POWER_1;
  for (int i = 0; i < 5; i++) sendRaw(receiverPower1);
  uint16_t receiverPower2[21] = IR_RECEIVER_POWER_2;
  for (int i = 0; i < 5; i++) sendRaw(receiverPower2);
}

void IrHandler::loop() {
  if (irrecv->decode(&results)) {
    Serial.println(F("[IR] >> Signal received:"));
    uint32_t now = millis();
    Serial.printf("Timestamp : %06u.%03u\n", now / 1000, now % 1000);
    if (results.overflow)
      Serial.println(F("WARNING: IR code is too big for buffer (1024). "
                    "This result shouldn't be trusted until this is resolved. "
                    "Edit & increase CAPTURE_BUFFER_SIZE.\n"));
    Serial.print(resultToHumanReadableBasic(&results));
    yield();  // Feed the WDT as the text output can take a while to print.
    Serial.println(resultToSourceCode(&results));
    yield();  // Feed the WDT (again)
    Serial.println(F("[IR] << Signal received"));
  }
}

