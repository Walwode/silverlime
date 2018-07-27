#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "switch.h"
#include "credentials.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"
#include "FastLED.h"

#define NUM_LEDS 176
#define DATA_PIN 3
CRGB leds[NUM_LEDS];
// 36

// prototypes
boolean connectWifi();

// on/off callbacks 
bool kitchenStripOn();
bool kitchenStripOff();

// Change this before you flash
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *kitchenStrip = NULL;

bool isKitchenStripOn = false;

void setup()
{
  Serial.begin(115200);
   
  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  if(wifiConnected){
    upnpBroadcastResponder.beginUdpMulticast();
    
    // Define your switches here. Max 10
    // Format: Alexa invocation name, local port no, on callback, off callback
    kitchenStrip = new Switch("Kitchen Strip", 80, kitchenStripOn, kitchenStripOff);

    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*kitchenStrip);
  }
  
  FastLED.setBrightness(200);
  // FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000); 
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  kitchenStripOn();
}
 
void loop()
{
	 if(wifiConnected){
      upnpBroadcastResponder.serverLoop();
      
      kitchenStrip->serverLoop();
	 }
}

void blackStrip() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
}

bool kitchenStripOn() {
  Serial.println("Switch kitchen strip on ...");
  
  isKitchenStripOn = true;
  blackStrip();
  for (int i = 0; i < 36; i++) {
    leds[i] = CRGB::White;
    // leds[NUM_LEDS - i - 1] = CRGB::White;;
  }
  FastLED.show();
  return isKitchenStripOn;
}

bool kitchenStripOff() {
  Serial.println("Switch kitchen strip off ...");

  isKitchenStripOn = false;
  blackStrip();
  FastLED.show();
  return isKitchenStripOn;
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
