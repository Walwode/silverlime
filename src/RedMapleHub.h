#ifndef PREDMAPLEHUB_H
#define PREDMAPLEHUB_H

#include <nRF24L01.h>
#include <RF24.h>
#include <Scheduler.h>
#include <SPI.h>

RF24 *radio = NULL;

struct SensorData {
  short version;
  short type;
  short value;
  char deviceId[9]; // esp char[] != nano char[] -> last in struct!
};

class RedMapleHub : public Task {
  
public:
  RedMapleHub() {};
  ~RedMapleHub() {};

  static bool restartRadio();
  static void sendDataToApi(SensorData* data);

protected:
  void loop();
  void setup();
  
};

void RedMapleHub::setup() {
  radio = new RF24(RF_CE_PIN, RF_CSN_PIN); // HSPI + pins 2 for CE and 15 for CSN
  
  restartRadio();
  radio->powerUp();
  radio->startListening();
}

void RedMapleHub::loop() {
  if (radio->available()) {

    uint8_t pipe;
    while (radio->available(&pipe)) {
      
      SensorData data;
      radio->read(&data, sizeof(data));
      switch (data.version) {
        case 1.0:
          Serial.println(F(">> Radio Data start"));
          Serial.println(data.version);
          Serial.println(data.deviceId);
          Serial.println(data.type);
          Serial.println(data.value);
          Serial.println(F("<< Radio Data end"));
          sendDataToApi(&data);
          break;
      }
    }
  }
  delay(500);
}

bool RedMapleHub::restartRadio() {
  Serial.print(F("Restart radio... "));
  // yield();

  radio->begin();
  radio->setChannel(2);
  radio->setPALevel(RF24_PA_LOW);      // RF24_PA_MIN = -18dBm, RF24_PA_LOW = -12dBm, RF24_PA_HIGH = -6dBm, RF24_PA_MAX = 0dBm
  radio->setDataRate(RF24_1MBPS);
  radio->setCRCLength(RF24_CRC_16);
  // radio->setAutoAck(1);                   // Ensure autoACK is enabled
  // radio->setRetries(2, 15);               // Optionally, increase the delay between retries. Want the number of auto-retries as high a

  radio->openWritingPipe(RF24_ADDRESS_ESP);
  radio->openReadingPipe(1, RF24_ADDRESS_UNO);
  radio->openReadingPipe(1, RF24_ADDRESS_NANO);
  radio->openReadingPipe(1, RF24_ADDRESS_PRO);
  // radio->enableDynamicPayloads();     // must have for multi pipe receiving

  radio->stopListening();
  radio->powerDown();
  Serial.println(F("done"));  
}

void RedMapleHub::sendDataToApi(SensorData* data) {
  Serial.println(F(">> Send by WiFi start"));
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(API_SERVER, httpPort)) {
    Serial.println(F("Connection failed"));
    return;
  }
  
  // We now create a URI for the request
  String url;
  url += API_SITE;
  url += F("?action=sensor&access=");
  url += API_ACCESS_TOKEN;
  url += "&device=" + String(data->deviceId);
  url += "&type=" + String(data->type);
  url += "&value=" + String(data->value);
  // String url = "/whitewalnut/plant-arduino.php?action=humidity&code=EXTERNAL&humidity=" + String(data->humidity); 
  
  Serial.print(F("Requesting URL: "));
  Serial.println(url);
  
  // This will send the request to the server
  client.print(F("GET "));
  client.print(url);
  client.print(F(" HTTP/1.1\r\n"));
  client.print(F("Host: "));
  client.print(API_SERVER);
  client.print(F("\r\n"));
  client.print(F("Connection: close\r\n\r\n"));
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(F("Client Timeout !"));
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println(F("Closing connection"));
  
  Serial.println(F("<< Send by WiFi end"));
}


#endif

