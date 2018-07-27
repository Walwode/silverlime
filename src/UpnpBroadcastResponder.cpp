#include "UpnpBroadcastResponder.h"
#include "Switch.h"
#include <functional>
 
// Multicast declarations
IPAddress ipMulti(239, 255, 255, 250);
const unsigned int portMulti = 1900;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];

#define MAX_SWITCHES 14
Switch switches[MAX_SWITCHES] = {};
int numOfSwitchs = 0;

//#define numOfSwitchs (sizeof(switches)/sizeof(Switch)) //array size  
 
//<<constructor>>
UpnpBroadcastResponder::UpnpBroadcastResponder(){
    
}
 
//<<destructor>>
UpnpBroadcastResponder::~UpnpBroadcastResponder(){/*nothing to destruct*/}
 
bool UpnpBroadcastResponder::beginUdpMulticast(){
  boolean state = false;
  
  Serial.println(F("Begin multicast .."));
  
  if(UDP.beginMulticast(WiFi.localIP(), ipMulti, portMulti)) {
    Serial.print(F("Udp multicast server started at "));
    Serial.print(ipMulti);
    Serial.print(F(":"));
    Serial.println(portMulti);

    state = true;
  }
  else{
    Serial.println(F("Connection failed"));
  }
  
  return state;
}

//Switch *ptrArray;

void UpnpBroadcastResponder::addDevice(Switch& device) {
  Serial.print(F("Adding switch : "));
  Serial.print(device.getAlexaInvokeName());
  Serial.print(F(" index : "));
  Serial.println(numOfSwitchs);
  
  switches[numOfSwitchs] = device;
  numOfSwitchs++;
}

void UpnpBroadcastResponder::serverLoop(){
  int packetSize = UDP.parsePacket();
  if (packetSize <= 0)
    return;
  
  IPAddress senderIP = UDP.remoteIP();
  unsigned int senderPort = UDP.remotePort();
  
  // read the packet into the buffer
  UDP.read(packetBuffer, packetSize);
  
  // check if this is a M-SEARCH for WeMo device
  String request = String((char *)packetBuffer);

  if(request.indexOf('M-SEARCH') >= 0) {
      // Issue https://github.com/kakopappa/arduino-esp8266-alexa-multiple-wemo-switch/issues/22 fix
      if((request.indexOf(F("urn:Belkin:device:**")) > 0) || (request.indexOf(F("ssdp:all")) > 0) || (request.indexOf(F("upnp:rootdevice")) > 0)) {
        Serial.print(F("Got UDP Belkin Request... "));
        
        // int arrSize = sizeof(switchs) / sizeof(Switch);
      
        for(int n = 0; n < numOfSwitchs; n++) {
            Switch &sw = switches[n];

            if (&sw != NULL) {
              sw.respondToSearch(senderIP, senderPort);
              Serial.print(sw.getAlexaInvokeName());
              Serial.print(F(", "));
            }
        }
        Serial.println();
      }
  }
}

