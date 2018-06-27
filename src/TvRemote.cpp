#include <LinkedList.h>
#include "Configuration.h"
#include "IrHandler.h"
#include "RemoteAction.h"
#include "RcHandler.h"
#include "TvRemote.h"

// 24sec Receiver off -> Receiver idle
// 11sec Receiver idle -> Receiver off
// 30sec Receiver idle -> Receiver on
// 70sec Receiver on -> Receiver idle

// 3sec TV off -> TV idle
// 2sec TV idle -> TV off
// 5sec TV on -> TV idle
// 10sec TV idle -> TV SmartHub

LinkedList<RemoteAction*> tvActionStack = LinkedList<RemoteAction*>();
bool isTvOn;
bool isNetflixOn;

bool TvRemote::turnTvOn() {
  Serial.println(F("Turn TV on"));
  tvActionStack.add(new RemoteAction(TV_POWER_ON, 3000));
  tvActionStack.add(new RemoteAction(SAMSUNG_POWER, 21000));
  tvActionStack.add(new RemoteAction(RECEIVER_POWER, 4000));
  return isTvOn = isNetflixOn = true;
}

bool TvRemote::turnTvOff() {
  Serial.println(F("Turn TV off"));
  tvActionStack.add(new RemoteAction(RECEIVER_POWER, 1000));
  tvActionStack.add(new RemoteAction(SAMSUNG_POWER, 69000));
  tvActionStack.add(new RemoteAction(TV_POWER_OFF, 4000));
  return isTvOn = isNetflixOn = false;
}

bool TvRemote::turnNetflixOn() {
  Serial.println(F("Turn Netflix on"));
  tvActionStack.add(new RemoteAction(TV_POWER_ON, 3000));
  tvActionStack.add(new RemoteAction(SAMSUNG_POWER, 10000));
  // tvActionStack.add(new RemoteAction(SAMSUNG_SMARTHUB, 2000));
  tvActionStack.add(new RemoteAction(SAMSUNG_OK));
  return isTvOn = isNetflixOn = true;
}

bool TvRemote::turnNetflixOff() {
  Serial.println(F("Turn Netflix off"));
  tvActionStack.add(new RemoteAction(SAMSUNG_POWER, 5000));
  tvActionStack.add(new RemoteAction(TV_POWER_OFF, 4000));
  return isTvOn = isNetflixOn = false;
}

void TvRemote::loop() {
  RemoteAction* action = tvActionStack.shift();
  if (action) {
    switch (action->getAction()) {
      case TV_POWER_ON: RcHandler::powerOnF1(); break;
      case TV_POWER_OFF: RcHandler::powerOffF1(); break;
      case SAMSUNG_POWER: IrHandler::samsungPower(); break;
      case SAMSUNG_SMARTHUB: IrHandler::samsungSmartHub(); break;
      case SAMSUNG_OK: IrHandler::samsungOk(); break;
      case RECEIVER_POWER: IrHandler::receiverPower(); break;
        // IrHandler::receiverPower1();
        // delay(200);
        // IrHandler::receiverPower2();
        // break;
    }
    delay(action->getDuration());
  } else delay(100);
}

