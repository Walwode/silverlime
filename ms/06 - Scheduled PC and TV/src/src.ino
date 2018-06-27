// blynk auth ecdfec7c8f8b447896337d1a5c783cb2

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "AlexaHandler.h"
#include "Credentials.h"
#include "IrHandler.h"
#include "PcRemote.h"
#include "RcHandler.h"
#include "Scheduler.h"
#include "TvRemote.h"

AlexaHandler alexaHandler;
PcRemote pcRemote;
TvRemote tvRemote;
bool connectWifi();

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;

void setup()
{
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  while(!connectWifi());
  IrHandler::setup(4); // esp8862 pin D2
  RcHandler::setup(16); // esp8862 pin D0
  
  Scheduler.start(&alexaHandler);
  Scheduler.start(&pcRemote);
  Scheduler.start(&tvRemote);
}

void loop() {
  Scheduler.begin();
  Serial.println("unreachable code");
}

bool connectWifi() {
  boolean state = true;
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10) {
      state = false;
      break;
    }
    i++;
  }

  if (state) {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }

  return state;
}

