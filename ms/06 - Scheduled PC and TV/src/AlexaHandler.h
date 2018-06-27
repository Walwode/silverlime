#ifndef ALEXAHANDLER_H
#define ALEXAHANDLER_H

#include <Scheduler.h>
#include "UpnpBroadcastResponder.h"

class AlexaHandler : public Task {
  UpnpBroadcastResponder upnpBroadcastResponder;
  Switch *pc = NULL;
  Switch *tv = NULL;
  Switch *netflix = NULL;
  
public:
  AlexaHandler() {};
  ~AlexaHandler() {};

  static bool pcOn();
  static bool pcOff();
  static bool tvOn();
  static bool tvOff();
  static bool netflixOn();
  static bool netflixOff();

protected:
  void loop();
  void setup();
};
#endif
