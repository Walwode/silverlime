#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <fauxmoESP.h>
#include <LinkedList.h>
#include "Configuration.h"
#include "IrHandler.h"
#include "RemoteAction.h"
#include "RcHandler.h"

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;

bool alexaCh1On();
bool alexaCh2On();
bool alexaCh3On();
bool alexaCh4On();
bool alexaTvOn();
bool alexaNetflixOn();
bool alexaCh1Off();
bool alexaCh2Off();
bool alexaCh3Off();
bool alexaCh4Off();
bool alexaTvOff();
bool alexaNetflixOff();

fauxmoESP fauxmo;

LinkedList<RemoteAction*> taskActionStack = LinkedList<RemoteAction*>();
RemoteAction *remoteAction = NULL;
unsigned long actionDelayUntil;

void setup() {
  Serial.begin(SERIAL_BAUD, SERIAL_8N1, SERIAL_TX_ONLY);
  
  while(!connectWifi());
  initializeAlexa();
  IrHandler::setup(IR_SND_PIN, IR_RCV_PIN);
  RcHandler::setup(RC_SND_PIN, RC_RCV_PIN);
}

void loop() {
  fauxmo.handle();
  
  // IrHandler::loop();
  // RcHandler::loop();
  doTaskActionStack();
  delay(10);
}

void doTaskActionStack() {
  if (!actionInDelay()) {
    remoteAction = taskActionStack.shift();
    if (remoteAction) {
      switch (remoteAction->getAction()) {
        case TV_POWER_ON: RcHandler::powerOnCh1(); break;
        case TV_POWER_OFF: RcHandler::powerOffCh1(); break;
        case SAMSUNG_POWER: IrHandler::samsungPower(); break;
        case SAMSUNG_SMARTHUB: IrHandler::samsungSmartHub(); break;
        case SAMSUNG_OK: IrHandler::samsungOk(); break;
        case RECEIVER_POWER: IrHandler::receiverPower(); break;
      }
      setActionDelay(remoteAction->getDuration());
    }
  }
}

bool actionInDelay() {
  if (actionDelayUntil <= millis()) actionDelayUntil = 0; // overflow
  return (actionDelayUntil > millis());
}

void setActionDelay(int duration) {
  actionDelayUntil = millis() + duration;
}

void initializeAlexa() {
  Serial.print(F("Initialize Alexa... "));
  
  fauxmo.enable(true);
  fauxmo.enable(false);
  fauxmo.enable(true);
  
  fauxmo.addDevice("POWPL-CH1");
  fauxmo.addDevice("POWPL-CH2");
  fauxmo.addDevice("POWPL-CH3");
  fauxmo.addDevice("POWPL-CH4");
  fauxmo.addDevice("TFLOW-TV");
  fauxmo.addDevice("TFLOW-NTFLX");

  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
    Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
    if (strcmp(device_name, "POWPL-CH1") == 0) {
      state ? alexaCh1On() : alexaCh1Off();
    }
    if (strcmp(device_name, "POWPL-CH2") == 0) {
      state ? alexaCh2On() : alexaCh2Off();
    }
    if (strcmp(device_name, "POWPL-CH3") == 0) {
      state ? alexaCh3On() : alexaCh3Off();
    }
    if (strcmp(device_name, "POWPL-CH4") == 0) {
      state ? alexaCh4On() : alexaCh4Off();
    }
    if (strcmp(device_name, "TFLOW-TV") == 0) {
      state ? alexaTvOn() : alexaTvOff();
    }
    if (strcmp(device_name, "TFLOW-NTFLX") == 0) {
      state ? alexaNetflixOn() : alexaNetflixOff();
    }
  });
  
  Serial.println(F("done"));
}

bool alexaCh1On() {
  Serial.println(F("Turn Channel 1 on"));
  RcHandler::powerOnCh1();
  return true;
}

bool alexaCh1Off() {
  Serial.println(F("Turn Channel 1 off"));
  RcHandler::powerOffCh1();
  return false;
}

bool alexaCh2On() {
  Serial.println(F("Turn Channel 2 on"));
  RcHandler::powerOnCh2();
  return true;
}

bool alexaCh2Off() {
  Serial.println(F("Turn Channel 2 off"));
  RcHandler::powerOffCh2();
  return false;
}

bool alexaCh3On() {
  Serial.println(F("Turn Channel 3 on"));
  RcHandler::powerOnCh3();
  return true;
}

bool alexaCh3Off() {
  Serial.println(F("Turn Channel 3 off"));
  RcHandler::powerOffCh3();
  return false;
}

bool alexaCh4On() {
  Serial.println(F("Turn Channel 4 on"));
  RcHandler::powerOnCh4();
  return true;
}

bool alexaCh4Off() {
  Serial.println(F("Turn Channel 4 off"));
  RcHandler::powerOffCh4();
  return false;
}

bool alexaTvOn() {
  Serial.println(F("Turn TV on"));
  taskActionStack.add(new RemoteAction(TV_POWER_ON, 3000));
  taskActionStack.add(new RemoteAction(SAMSUNG_POWER, 21000));
  taskActionStack.add(new RemoteAction(RECEIVER_POWER, 4000));
  return true;
}

bool alexaTvOff() {
  Serial.println(F("Turn TV off"));
  taskActionStack.add(new RemoteAction(RECEIVER_POWER, 1000));
  taskActionStack.add(new RemoteAction(SAMSUNG_POWER, 69000));
  taskActionStack.add(new RemoteAction(TV_POWER_OFF, 4000));
  return false;
}

bool alexaNetflixOn() {
  Serial.println(F("Turn Netflix on"));
  taskActionStack.add(new RemoteAction(TV_POWER_ON, 3000));
  taskActionStack.add(new RemoteAction(SAMSUNG_POWER, 10000));
  // taskActionStack.add(new RemoteAction(SAMSUNG_SMARTHUB, 2000));
  taskActionStack.add(new RemoteAction(SAMSUNG_OK));
  return true;
}

bool alexaNetflixOff() {
  Serial.println(F("Turn Netflix off"));
  taskActionStack.add(new RemoteAction(SAMSUNG_POWER, 5000));
  taskActionStack.add(new RemoteAction(TV_POWER_OFF, 4000));
  return false;
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

