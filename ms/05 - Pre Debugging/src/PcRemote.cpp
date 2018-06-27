#include <LinkedList.h>
#include "IrHandler.h"
#include "PcRemote.h"
#include "RemoteAction.h"
#include "RemoteCodes.h"
#include "RcHandler.h"

LinkedList<RemoteAction*> pcActionStack = LinkedList<RemoteAction*>();
bool isPcOn;

bool PcRemote::turnPcOn() {
  Serial.println(F("Turn PC on"));
  pcActionStack.add(new RemoteAction(PC_POWER_ON));
  return isPcOn = true;
}

bool PcRemote::turnPcOff() {
  Serial.println(F("Turn PC off"));
  pcActionStack.add(new RemoteAction(PC_POWER_OFF));
  return isPcOn = false;
}

void PcRemote::loop() {
  RemoteAction* action = pcActionStack.shift();
  if (action) {
    Serial.print(F("Process Action "));
    Serial.println(action->getAction());
    switch (action->getAction()) {
      case PC_POWER_ON: RcHandler::powerOnF2(); break;
      case PC_POWER_OFF: RcHandler::powerOffF2(); break;
    }
    delay(action->getDuration());
  } else delay(100);
}

