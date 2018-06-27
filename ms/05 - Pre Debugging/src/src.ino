// blynk auth ecdfec7c8f8b447896337d1a5c783cb2

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
// #include "AlexaHandler.h"
// #include "Credentials.h"
// #include "IrHandler.h"
// #include "PcRemote.h"
// #include "RcHandler.h"
// #include "Scheduler.h"
// #include "TvRemote.h"

#include <IRremoteESP8266.h>
#include <IRsend.h>

// AlexaHandler alexaHandler;
// PcRemote pcRemote;
// TvRemote tvRemote;
bool connectWifi();

// const char* ssid = WIFI_SSID;
// const char* password = WIFI_PASS;

void setup()
{
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  while(!connectWifi());
  // IrHandler::setup(4); // esp8862 pin D2
  // RcHandler::setup(16); // esp8862 pin D0
  
  // Scheduler.start(&alexaHandler);
  // Scheduler.start(&pcRemote);
  // Scheduler.start(&tvRemote);
}

void loop() {
  // Scheduler.begin();
  Serial.println("unreachable code");
  Serial.println("IR Samsung Power");
  IRsend irsend2(4);  // Set the GPIO to be used to sending the message.
  uint16_t samsungPower[135] = {4580, 4432,  628, 1614,  628, 1612,  628, 1610,  626, 492,  626, 490,  630, 492,  626, 492,  628, 494,  624, 1614,  624, 1640,  602, 1610,  628, 516,  604, 490,  628, 492,  628, 490,  630, 490,  628, 492,  628, 1612,  626, 494,  626, 494,  626, 494,  626, 490,  628, 492,  628, 492,  626, 1612,  628, 490,  630, 1612,  626, 1612,  628, 1610,  626, 1614,  626, 1614,  624, 1612,  628, 46746,  4582, 4434,  624, 1614,  624, 1612,  626, 1614,  624, 492,  628, 492,  628, 490,  628, 492,  628, 494,  624, 1612,  628, 1610,  626, 1616,  624, 494,  626, 492,  628, 492,  626, 494,  626, 492,  626, 496,  624, 1614,  628, 490,  628, 490,  628, 492,  628, 492,  628, 490,  628, 492,  628, 1612,  626, 494,  626, 1612,  626, 1610,  628, 1612,  626, 1614,  626, 1614,  626, 1610,  630};  // SAMSUNG E0E040BF
  irsend2.sendRaw(samsungPower, 67, 38);  // Send a raw data capture at 38kHz.  
  delay(5000);
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

