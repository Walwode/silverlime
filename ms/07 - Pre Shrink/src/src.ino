// blynk auth ecdfec7c8f8b447896337d1a5c783cb2

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "AlexaHandler.h"
#include "Configuration.h"
#include "IrHandler.h"
#include "PcRemote.h"
#include "RedMapleHub.h"
#include "RcHandler.h"
#include "Scheduler.h"
#include "TvRemote.h"

AlexaHandler alexaHandler;
PcRemote pcRemote;
RedMapleHub redMapleHub;
TvRemote tvRemote;
bool connectWifi();

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;

void setup()
{
  Serial.begin(SERIAL_BAUD, SERIAL_8N1, SERIAL_TX_ONLY);
  while(!connectWifi());
  IrHandler::setup(IR_SND_PIN, IR_RCV_PIN);
  RcHandler::setup(RC_SND_PIN, RC_RCV_PIN);
  
  Scheduler.start(&alexaHandler);
  Scheduler.start(&pcRemote);
  // Scheduler.start(&redMapleHub);
  Scheduler.start(&tvRemote);
}

void loop() {
  Scheduler.begin();
  Serial.println(F("unreachable code"));
}

bool connectWifi() {
  boolean state = true;
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.println(F("Connecting to WiFi"));

  // Wait for connection
  Serial.print(F("Connecting ..."));
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
    if (i > 10) {
      state = false;
      break;
    }
    i++;
  }

  if (state) {
    Serial.println();
    Serial.print(F("Connected to "));
    Serial.println(ssid);
    Serial.print(F("IP address: "));
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println();
    Serial.println(F("Connection failed."));
  }

  return state;
}

