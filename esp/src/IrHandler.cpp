#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRutils.h>
#include "config.h"
#include "IrHandler.h"

#define CAPTURE_BUFFER_SIZE 1024

IRsend *irsend = NULL; // Set the GPIO to be used to sending the message
IRrecv *irrecv = NULL; // set the GPIO to be used to reveice the message
decode_results results;  // Somewhere to store the results

void IrHandler::setup(byte sndPin, byte rcvPin) {
  Serial.print(F("Initialize infrared... "));
  irsend = new IRsend(sndPin);
  irsend->begin();

  irrecv = new IRrecv(rcvPin, CAPTURE_BUFFER_SIZE, 15U, true);
  irrecv->setUnknownThreshold(12); // Ignore messages with less than minimum on or off pulses.
  irrecv->enableIRIn();  // Start the receiver
  Serial.println(F("done"));
}

void IrHandler::sendRaw(uint16_t rawData[]) {
  noInterrupts();
  irsend->sendRaw(rawData, sizeof(rawData), 38);  // Send a raw data capture at 38kHz.  
  interrupts();
  yield();
}

void IrHandler::samsungPower() {
  Serial.println(F("IR Samsung Power"));
  irsend->sendSAMSUNG(IR_SAMSUNG_POWER);
}

void IrHandler::samsungSmartHub() {
  Serial.println(F("IR Samsung Smart Hub"));
  irsend->sendSAMSUNG(IR_SAMSUNG_SMARTHUB);
}

void IrHandler::samsungOk() {
  Serial.println(F("IR Samsung OK"));
  irsend->sendSAMSUNG(IR_SAMSUNG_OK);
}

void IrHandler::receiverPower() {
  Serial.println(F("IR Receiver Power"));
  irsend->sendRC5(IR_RECEIVER_POWER_1);
  irsend->sendRC5(IR_RECEIVER_POWER_2);
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

