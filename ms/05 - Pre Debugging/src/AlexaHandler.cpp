#include "AlexaHandler.h"
#include "PcRemote.h"
#include "TvRemote.h"
#include "Switch.h"

void AlexaHandler::setup() {
  upnpBroadcastResponder.beginUdpMulticast();

  // Define your switches here. Max 10
  // Format: Alexa invocation name, local port no, on callback, off callback
  pc = new Switch("Computer Steckdose", 81, pcOn, pcOff);
  tv = new Switch("Fernsehr", 85, tvOn, tvOff);
  netflix = new Switch("Netflix", 86, netflixOn, netflixOff);

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

