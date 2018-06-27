#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "Switch.h"
#include "Credentials.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"
#include <RCSwitch.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include "RemoteCodes.h"
#include "StensTimer.h"
#include <LinkedList.h>

struct TimerHelper : public IStensTimerListener {
  public:
    void updateApi();
    void timerCallback(Timer* timer);
};

// prototypes
boolean connectWifi();

// on/off callbacks
bool TvPowerOn();
bool TvPowerOff();
bool ComputerPowerOn();
bool ComputerPowerOff();
bool TvOn();
bool TvOff();
bool NetflixOn();
bool NetflixOff();

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *tvPower = NULL;
Switch *computerPower = NULL;
Switch *tv = NULL;
Switch *netflix = NULL;
RCSwitch powerPlugs = RCSwitch();
StensTimer* stensTimer;
TimerHelper apiTimer;

class RemoteAction {
  int action;
  int duration;
  
  RemoteAction(int action);
  RemoteAction(int action, int duration);
  ~RemoteAction();
  
  int getAction();
  int getDuration();
};

RemoteAction::RemoteAction(int action) {
  this.action = action;
  this.duration = 100;
}

RemoteAction::RemoteAction(int action, int duration) {
  this.action = action;
  this.duration = duration;
}

LinkedList<RemoteAction*> tvActionStack = LinkedList<RemoteAction*>();

bool isTvPowerOn = false;
bool isComputerPowerOn = false;
bool isTvOn = false;
bool isNetflixOn = false;

#define IR_LED 4  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRsend irsend(IR_LED);  // Set the GPIO to be used to sending the message.

void setup()
{
  irsend.begin();
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);

  // Initialise wifi connection
  wifiConnected = connectWifi();

  if (wifiConnected) {
    upnpBroadcastResponder.beginUdpMulticast();

    // Define your switches here. Max 10
    // Format: Alexa invocation name, local port no, on callback, off callback
    tvPower = new Switch("TV Steckdose", 80, tvPowerOn, tvPowerOff);
    computerPower = new Switch("Computer Steckdose", 81, computerPowerOn, computerPowerOff);
    tv = new Switch("Fernsehr", 85, tvOn, tvOff);
    netflix = new Switch("Netflix", 86, netflixOn, netflixOff);

    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*tvPower);
    upnpBroadcastResponder.addDevice(*computerPower);
    upnpBroadcastResponder.addDevice(*tv);
    upnpBroadcastResponder.addDevice(*netflix);

    // init timer
    stensTimer = StensTimer::getInstance();
    // StensTimer::getInstance()->setInterval(&apiTimer, ACTION_PLANT_UPDATE, 60000);
  }

  // 433MHz device
  Serial.println("Initialize 433MHz switches");
  powerPlugs.enableTransmit(16); // pin D0
  powerPlugs.setProtocol(1);
  powerPlugs.setPulseLength(185);
}

void loop()
{
  if (wifiConnected) {
    upnpBroadcastResponder.serverLoop();

    tvPower->serverLoop();
    computerPower->serverLoop();
    tv->serverLoop();
    netflix->serverLoop();
  }
  stensTimer->run();
}

void tvTimerCallback(Timer* timer) {
  switch (timer->getAction()) {
    case RECEIVER_POWER:
      updateApi();
      break;
      SAMSUNG_POWER
      POWER_OFF_F1
      SAMSUNG_SMARTHUB);
      SAMSUNG_OK;
  }
  
  RemoteAction action = tvActionStack.shift();
  if (action != null) processAction(action);
}

void processAction(RemoteAction& action) {
  tvTimer->setTimer(action.getAction(), action.getDuration());
}

bool tvPowerOn() {
  Serial.println("Switch TV power plug on ...");
  return switchTvPower(true);
}

bool tvPowerOff() {
  Serial.println("Switch TV power plug off ...");
  return switchTvPower(false);
}

bool computerPowerOn() {
  Serial.println("Switch computer power plug on ...");
  return switchComputerPower(true);
}

bool computerPowerOff() {
  Serial.println("Switch computer power plug off ...");
  return switchComputerPower(false);
}

bool tvOn() {
  Serial.println("Switch TV and receiver on ...");

  tvStack->add(POWER_ON_F1);
  tvStack->add(BREAK, 2000);
  tvStack->add(SAMSUNG_POWER);
  tvStack->add(BREAK, 23000);
  tvStack->add(RECEIVER_POWER);
  
  isTvOn = true;
  return isTvOn;
}

bool tvOff() {
  Serial.println("Switch TV and receiver off ...");

  tvStack->add(RECEIVER_POWER);
  tvStack->add(BREAK, 1000);
  tvStack->add(SAMSUNG_POWER);
  tvStack->add(BREAK, 11000);
  tvStack->add(POWER_OFF_F1);
  
  isTvOn = false;
  return isTvOn;
}

bool netflixOn() {
  Serial.println("Switch TV and netflix on ...");

  tvStack->add(POWER_ON_F1);
  tvStack->add(BREAK, 2000);
  tvStack->add(SAMSUNG_POWER);
  tvStack->add(BREAK, 11000);
  tvStack->add(SAMSUNG_SMARTHUB);
  tvStack->add(BREAK, 2000);
  tvStack->add(SAMSUNG_OK);
  
  isNetflixOn = true;
  return isNetflixOn;
}

bool netflixOff() {
  Serial.println("Switch TV and netflix off ...");

  tvStack->add(SAMSUNG_POWER);
  tvStack->add(BREAK, 5000);
  tvStack->add(POWER_OFF_F1);
  
  isNetflixOn = false;
  return isNetflixOn;
}

bool switchTvPower(bool tvPowerOn) {
  isTvPowerOn = tvPowerOn;
  if (tvPowerOn) powerPlugs.send("010001000001010100110011");
  else powerPlugs.send("010001000001010100111100");
  Serial.print("TV power plug new state: ");
  Serial.println(isTvPowerOn);
  return isTvPowerOn;
}

bool switchComputerPower(bool computerPowerOn) {
  isComputerPowerOn = computerPowerOn;
  if (computerPowerOn) powerPlugs.send("010001000001010111000011");
  else powerPlugs.send("010001000001010111001100");
  Serial.print("Computer power plug new state: ");
  Serial.println(isComputerPowerOn);
  return isComputerPowerOn;
}

void switchTvRemoteOn(bool tvRemoteOn) {
  irsend.sendRaw(samsungPower, 67, 38); // Send a raw data capture at 38kHz.
  Serial.print("TV remote button power");
}

void switchReceiverRemoteOn(bool receiverRemoteOn) {
  receiverPowerSignal1();
  delay(200);
  receiverPowerSignal2();
  Serial.print("Receiver remote button power");
}

void receiverPowerSignal1() {
  irsend.sendRaw(rawData, 67, 38);  // Send a raw data capture at 38kHz.
}

void receiverPowerSignal2() {
  uint16_t rawData[21] = {988, 800,  986, 796,  1904, 1666,  1904, 796,  988, 796,  988, 822,  962, 796,  988, 1696,  960, 798,  1900, 798,  990};  // RC5 A0C
  irsend.sendRaw(rawData, 67, 38);  // Send a raw data capture at 38kHz.
}

void remoteActionSmartHub() {
  uint16_t rawData[135] = {4578, 4430,  628, 1612,  626, 1608,  630, 1610,  630, 490,  628, 490,  628, 492,  628, 492,  628, 490,  628, 1612,  628, 1638,  600, 1614,  626, 492,  628, 490,  628, 490,  630, 490,  630, 490,  628, 1610,  630, 490,  628, 490,  628, 1614,  626, 1610,  628, 1612,  626, 1610,  628, 496,  624, 490,  628, 1610,  628, 1636,  602, 490,  628, 492,  628, 492,  628, 490,  628, 1618,  620, 46740,  4580, 4426,  630, 1636,  602, 1610,  628, 1610,  628, 492,  628, 490,  628, 488,  630, 494,  626, 486,  634, 1636,  602, 1610,  630, 1612,  626, 492,  628, 486,  632, 492,  628, 492,  628, 492,  626, 1610,  628, 490,  628, 492,  626, 1610,  630, 1610,  628, 1608,  630, 1610,  628, 492,  628, 486,  632, 1608,  628, 1610,  628, 492,  626, 492,  628, 488,  630, 492,  628, 1608,  632};  // SAMSUNG E0E09E61
  irsend.sendRaw(rawData, 67, 38);  // Send a raw data capture at 38kHz.
  Serial.print("TV remote button smart hub");
}

void remoteActionOk() {
  uint16_t rawData[135] = {4580, 4430,  626, 1614,  624, 1612,  628, 1612,  626, 492,  626, 490,  630, 492,  628, 490,  628, 492,  626, 1612,  628, 1612,  624, 1614,  624, 492,  626, 494,  626, 492,  626, 494,  626, 492,  626, 494,  626, 492,  628, 490,  630, 1612,  628, 492,  626, 1612,  626, 1612,  626, 492,  626, 1612,  626, 1638,  600, 1612,  628, 492,  626, 1612,  628, 492,  628, 490,  628, 1612,  628, 46748,  4580, 4430,  630, 1608,  630, 1610,  630, 1610,  628, 490,  628, 492,  628, 490,  628, 492,  628, 490,  628, 1616,  624, 1612,  626, 1612,  628, 492,  626, 492,  628, 492,  628, 492,  626, 492,  628, 488,  628, 492,  630, 488,  630, 1638,  600, 490,  628, 1612,  628, 1612,  626, 492,  628, 1614,  626, 1612,  628, 1610,  628, 490,  628, 1638,  600, 492,  628, 494,  624, 1612,  626};  // SAMSUNG E0E016E9
  irsend.sendRaw(rawData, 67, 38);  // Send a raw data capture at 38kHz.
  Serial.print("TV remote button ok");
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi() {
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
