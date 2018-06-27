#ifndef ALEXAHANDLER_H
#define ALEXAHANDLER_H

#include <Scheduler.h>
#include "PcRemote.h"
#include "TvRemote.h"
#include "Switch.h"
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

void AlexaHandler::setup() {
  upnpBroadcastResponder.beginUdpMulticast();

  // Define your switches here. Max 10
  // Format: Alexa invocation name, local port no, on callback, off callback
  pc = new Switch(F("Computer Steckdose"), 81, pcOn, pcOff);
  tv = new Switch(F("Fernsehr"), 85, tvOn, tvOff);
  netflix = new Switch(F("Netflix"), 86, netflixOn, netflixOff);

  Serial.println(F("Adding switches upnp broadcast responder"));
  upnpBroadcastResponder.addDevice(*pc);
  upnpBroadcastResponder.addDevice(*tv);
  upnpBroadcastResponder.addDevice(*netflix);
}

void AlexaHandler::loop() {
  upnpBroadcastResponder.serverLoop();
  pc->serverLoop();
  tv->serverLoop();
  netflix->serverLoop();
}

bool AlexaHandler::pcOn() {
  return PcRemote::turnPcOn();
}

bool AlexaHandler::pcOff() {
  return PcRemote::turnPcOff();
}

bool AlexaHandler::tvOn() {
  return TvRemote::turnTvOn();
}

bool AlexaHandler::tvOff() {
  return TvRemote::turnTvOff();
}

bool AlexaHandler::netflixOn() {
  return TvRemote::turnNetflixOn();
}

bool AlexaHandler::netflixOff() {
  return TvRemote::turnNetflixOff();
}

#endif
