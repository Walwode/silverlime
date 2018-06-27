#include <LinkedList.h>
#include "IrHandler.h"
#include "RemoteAction.h"
#include "RemoteCodes.h"
#include "RcHandler.h"
#include "TvRemote.h"

LinkedList<RemoteAction*> tvActionStack = LinkedList<RemoteAction*>();
bool isTvOn;
bool isNetflixOn;

bool TvRemote::turnTvOn() {
  Serial.println(F("Turn TV on"));
  // tvActionStack.add(new RemoteAction(TV_POWER_ON, 5000));
  // tvActionStack.add(new RemoteAction(SAMSUNG_POWER, 1000));
  tvActionStack.add(new RemoteAction(RECEIVER_POWER));
  return isTvOn = isNetflixOn = true;
}

bool TvRemote::turnTvOff() {
  Serial.println(F("Turn TV off"));
  tvActionStack.add(new RemoteAction(RECEIVER_POWER));
  // tvActionStack.add(new RemoteAction(SAMSUNG_POWER, 1000));
  // tvActionStack.add(new RemoteAction(TV_POWER_OFF));
  return isTvOn = isNetflixOn = false;
}

bool TvRemote::turnNetflixOn() {
  Serial.println(F("Turn Netflix on"));
  // tvActionStack.add(new RemoteAction(TV_POWER_ON, 5000));
  tvActionStack.add(new RemoteAction(SAMSUNG_POWER));
  // tvActionStack.add(new RemoteAction(SAMSUNG_SMART_HUB, 2000));
  // tvActionStack.add(new RemoteAction(SAMSUNG_OK));
  return isTvOn = isNetflixOn = true;
}

bool TvRemote::turnNetflixOff() {
  Serial.println(F("Turn Netflix off"));
  tvActionStack.add(new RemoteAction(SAMSUNG_POWER));
  // tvActionStack.add(new RemoteAction(TV_POWER_OFF));
  return isTvOn = isNetflixOn = false;
}

void TvRemote::loop() {
  RemoteAction* action = tvActionStack.shift();
  if (action) {
    switch (action->getAction()) {
      case TV_POWER_ON: RcHandler::powerOnF1(); break;
      case TV_POWER_OFF: RcHandler::powerOffF1(); break;
      case SAMSUNG_POWER: IrHandler::samsungPower(); break;
      case SAMSUNG_SMART_HUB: IrHandler::samsungSmartHub(); break;
      case SAMSUNG_OK: IrHandler::samsungOk(); break;
      case RECEIVER_POWER:
        IrHandler::receiverPower1();
        delay(200);
        IrHandler::receiverPower2();
        break;
    }
    delay(action->getDuration());
  } else delay(100);
}

