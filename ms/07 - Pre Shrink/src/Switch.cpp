#include "Switch.h"
#include "CallbackFunction.h"



//<<constructor>>
Switch::Switch() {
  Serial.println(F("default constructor called"));
}
//Switch::Switch(String alexaInvokeName,unsigned int port){
Switch::Switch(String alexaInvokeName, unsigned int port, CallbackFunction oncb, CallbackFunction offcb) {
  uint32_t chipId = ESP.getChipId();
  char uuid[64];
  sprintf_P(uuid, PSTR("38323636-4558-4dda-9188-cda0e6%02x%02x%02x"),
            (uint16_t) ((chipId >> 16) & 0xff),
            (uint16_t) ((chipId >>  8) & 0xff),
            (uint16_t)   chipId        & 0xff);

  serial = String(uuid);
  persistent_uuid = "Socket-1_0-" + serial + "-" + String(port);

  device_name = alexaInvokeName;
  localPort = port;
  onCallback = oncb;
  offCallback = offcb;

  startWebServer();
}



//<<destructor>>
Switch::~Switch() {
  /*nothing to destruct*/
}

void Switch::serverLoop() {
  if (server != NULL) {
    server->handleClient();
    delay(1);
  }
}

void Switch::startWebServer() {
  server = new ESP8266WebServer(localPort);

  server->on(F("/"), [&]() {
    handleRoot();
  });


  server->on(F("/setup.xml"), [&]() {
    handleSetupXml();
  });

  server->on(F("/upnp/control/basicevent1"), [&]() {
    handleUpnpControl();
  });

  server->on(F("/eventservice.xml"), [&]() {
    handleEventservice();
  });

  //server->onNotFound(handleNotFound);
  server->begin();

  Serial.print(F("WebServer started on port: "));
  Serial.println(localPort);
}

void Switch::handleEventservice() {
  Serial.println(F(" ########## Responding to eventservice.xml ... ########\n"));

  String eventservice_xml = F("<scpd xmlns=\"urn:Belkin:service-1-0\">"
                              "<actionList>"
                              "<action>"
                              "<name>SetBinaryState</name>"
                              "<argumentList>"
                              "<argument>"
                              "<retval/>"
                              "<name>BinaryState</name>"
                              "<relatedStateVariable>BinaryState</relatedStateVariable>"
                              "<direction>in</direction>"
                              "</argument>"
                              "</argumentList>"
                              "</action>"
                              "<action>"
                              "<name>GetBinaryState</name>"
                              "<argumentList>"
                              "<argument>"
                              "<retval/>"
                              "<name>BinaryState</name>"
                              "<relatedStateVariable>BinaryState</relatedStateVariable>"
                              "<direction>out</direction>"
                              "</argument>"
                              "</argumentList>"
                              "</action>"
                              "</actionList>"
                              "<serviceStateTable>"
                              "<stateVariable sendEvents=\"yes\">"
                              "<name>BinaryState</name>"
                              "<dataType>Boolean</dataType>"
                              "<defaultValue>0</defaultValue>"
                              "</stateVariable>"
                              "<stateVariable sendEvents=\"yes\">"
                              "<name>level</name>"
                              "<dataType>string</dataType>"
                              "<defaultValue>0</defaultValue>"
                              "</stateVariable>"
                              "</serviceStateTable>"
                              "</scpd>\r\n"
                              "\r\n");

  server->send(200, F("text/plain"), eventservice_xml.c_str());
}

void Switch::handleUpnpControl() {
  // Serial.println(F("########## Responding to  /upnp/control/basicevent1 ... ##########"));

  //for (int x=0; x <= HTTP.args(); x++) {
  //  Serial.println(HTTP.arg(x));
  //}

  String request = server->arg(0);
  /*
    Serial.print(F("request:"));
    Serial.println(request);
  */

  if (request.indexOf(F("SetBinaryState")) >= 0) {
    if (request.indexOf(F("<BinaryState>1</BinaryState>")) >= 0) {
      Serial.println(F("Got Turn on request"));
      switchStatus = onCallback();

      sendRelayState();
    }

    if (request.indexOf(F("<BinaryState>0</BinaryState>")) >= 0) {
      Serial.println(F("Got Turn off request"));
      switchStatus = offCallback();

      sendRelayState();
    }
  }

  if (request.indexOf(F("GetBinaryState")) >= 0) {
    Serial.println(F("Got binary state request"));
    sendRelayState();
  }

  server->send(200, F("text/plain"), F(""));
}

void Switch::handleRoot() {
  server->send(200, F("text/plain"), F("You should tell Alexa to discover devices"));
}

void Switch::handleSetupXml() {
  // Serial.println(F(" ########## Responding to setup.xml ... ########\n"));
  yield();

  IPAddress localIP = WiFi.localIP();
  char s[16];
  sprintf(s, "%d.%d.%d.%d", localIP[0], localIP[1], localIP[2], localIP[3]);

  String part1 = F("<?xml version=\"1.0\"?>"
                   "<root>"
                   "<device>"
                   "<deviceType>urn:Belkin:device:controllee:1</deviceType>"
                   "<friendlyName>");
                     
  String part2 = F("</friendlyName>"
                   "<manufacturer>Belkin International Inc.</manufacturer>"
                   "<modelName>Socket</modelName>"
                   "<modelNumber>3.1415</modelNumber>"
                   "<modelDescription>Belkin Plugin Socket 1.0</modelDescription>\r\n"
                   "<UDN>uuid:");
                     
  String part3 = F("</UDN>"
                   "<serialNumber>221517K0101769</serialNumber>"
                   "<binaryState>0</binaryState>"
                   "<serviceList>"
                   "<service>"
                   "<serviceType>urn:Belkin:service:basicevent:1</serviceType>"
                   "<serviceId>urn:Belkin:serviceId:basicevent1</serviceId>"
                   "<controlURL>/upnp/control/basicevent1</controlURL>"
                   "<eventSubURL>/upnp/event/basicevent1</eventSubURL>"
                   "<SCPDURL>/eventservice.xml</SCPDURL>"
                   "</service>"
                   "</serviceList>"
                   "</device>"
                   "</root>\r\n"
                   "\r\n");

  String setup_xml = part1 + device_name + part2 + persistent_uuid + part3;

  server->send(200, F("text/xml"), setup_xml.c_str());
  Serial.print(F("Responding setup.xml for"));
  Serial.println(device_name);

  /*
    Serial.print(F("Sending :"));
    Serial.println(setup_xml);
  */
}

String Switch::getAlexaInvokeName() {
  return device_name;
}

void Switch::sendRelayState() {
  String body =
    F("<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body>\r\n"
      "<u:GetBinaryStateResponse xmlns:u=\"urn:Belkin:service:basicevent:1\">\r\n"
      "<BinaryState>");

  body += (switchStatus ? "1" : "0");

  body += F("</BinaryState>\r\n"
            "</u:GetBinaryStateResponse>\r\n"
            "</s:Body> </s:Envelope>\r\n");

  server->send(200, F("text/xml"), body.c_str());

  /*
    Serial.print(F("Sending :"));
    Serial.println(body);
  */
}

void Switch::respondToSearch(IPAddress& senderIP, unsigned int senderPort) {
  /*
    Serial.println("");
    Serial.print(F("Sending response to "));
    Serial.println(senderIP);
    Serial.print(F("Port : "));
    Serial.println(senderPort);
  */

  IPAddress localIP = WiFi.localIP();
  char s[16];
  sprintf(s, "%d.%d.%d.%d", localIP[0], localIP[1], localIP[2], localIP[3]);

  String part1 =
    F("HTTP/1.1 200 OK\r\n"
    "CACHE-CONTROL: max-age=86400\r\n"
    "DATE: Sat, 26 Nov 2016 04:56:29 GMT\r\n"
    "EXT:\r\n"
    "LOCATION: http://");

  String part2 =
    F("/setup.xml\r\n"
    "OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n"
    "01-NLS: b9200ebb-736d-4b93-bf03-835149d13983\r\n"
    "SERVER: Unspecified, UPnP/1.0, Unspecified\r\n"
    "ST: urn:Belkin:device:**\r\n"
    "USN: uuid:");

  String part3 =
    F("::urn:Belkin:device:**\r\n"
    "X-User-Agent: redsonic\r\n\r\n");

  String response = part1 + String(s) + ":" + String(localPort) + part2 + persistent_uuid + part3;

  UDP.beginPacket(senderIP, senderPort);
  UDP.write(response.c_str());
  UDP.endPacket();

  /* Serial.println(F("Response sent !")); */
}
