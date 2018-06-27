#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "switch.h"
#include "credentials.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"

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

// Change this before you flash
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *tvPower = NULL;
Switch *computerPower = NULL;
Switch *tv = NULL;
Switch *netflix = NULL;

bool isTvPowerOn = false;
bool isComputerPowerOn = false;
bool isTvOn = false;
bool isNetflixOn = false;

void setup()
{
  Serial.begin(9600);
   
  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  if(wifiConnected){
    upnpBroadcastResponder.beginUdpMulticast();
    
    // Define your switches here. Max 10
    // Format: Alexa invocation name, local port no, on callback, off callback
    tvPower = new Switch("TV Steckdose", 80, tvPowerOn, tvPowerOn);
    computerPower = new Switch("Computer Steckdose", 81, computerPowerOn, computerPowerOff);
    tv = new Switch("Fernsehr", 85, tvOn, tvOff);
    netflix = new Switch("Netflix", 86, netflixOn, netflixOff);

    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*tvPower);
    upnpBroadcastResponder.addDevice(*computerPower);
    upnpBroadcastResponder.addDevice(*tv);
    upnpBroadcastResponder.addDevice(*netflix);
  }
}
 
void loop()
{
	 if(wifiConnected){
      upnpBroadcastResponder.serverLoop();
      
      tvPower->serverLoop();
      computerPower->serverLoop();
      tv->serverLoop();
      netflix->serverLoop();
	 }
}

bool tvPowerOn() {
    Serial.println("Switch TV power plug on ...");
    
    isTvPowerOn = true;    
    return isTvPowerOn;
}

bool tvPowerOff() {
    Serial.println("Switch TV power plug off ...");

    isTvPowerOn = false;
    return isTvPowerOn;
}

bool computerPowerOn() {
    Serial.println("Switch computer power plug on ...");

    isComputerPowerOn = true;
    return isComputerPowerOn;
}

bool computerPowerOff() {
    Serial.println("Switch computer power plug off ...");

  isComputerPowerOn = false;
  return isComputerPowerOn;
}

bool tvOn() {
    Serial.println("Switch TV and receiver on ...");

    isTvOn = true;
    return isTvOn;
}

bool tvOff() {
    Serial.println("Switch TV and receiver off ...");

  isTvOn = false;
  return isTvOn;
}

bool netflixOn() {
    Serial.println("Switch TV and netflix on ...");

    isNetflixOn = true;
    return isNetflixOn;
}

bool netflixOff() {
    Serial.println("Switch TV and netflix off ...");

  isNetflixOn = false;
  return isNetflixOn;
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
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
    if (i > 10){
      state = false;
      break;
    }
    i++;
  }
  
  if (state){
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
