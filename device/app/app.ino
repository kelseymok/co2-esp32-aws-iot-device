#include <ArduinoJson.h>

#include "SensorReader.h"
#include "AWSClient.h"
#include "CertClient.h"
#include "AppConfigClient.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <HardwareSerial.h>

#include <string>

#include "SPIFFS.h"


#include <NTPClient.h>
#include <WiFiUdp.h>

// NTP settings
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org");
WiFiClientSecure* net;
MQTTClient* client;
SensorReader SReader;
AWSClient* awsClient;
CertClient* certClient;
AppConfigClient* configClient;

char jsonBuffer[512];
int reading;

void connectWifi() {
   WiFi.mode(WIFI_STA);
   WiFi.begin(configClient->wifiSsid.c_str(), configClient->wifiPassword.c_str());

  Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
}

char* buildMessage(int reading, int epoch) {
  StaticJsonDocument<200> doc;
  doc["epoch_time"] = epoch;
  doc["co2_ppm"] = reading;
  doc["device_id"] = configClient->thingName;
  serializeJson(doc, jsonBuffer);
  return jsonBuffer;
}

void setSystemTime() {
    timeClient.begin();
    timeClient.setTimeOffset(3600);
}



void setup() {
  Serial.begin(9600);
  while (!Serial) ;
  Serial.println("Setting up.....");
  configClient = new AppConfigClient("/config.json");
  Serial.println("created app config client");
  configClient->begin();
  Serial.println("began");

  Serial.println("creating network clients");
   net = new WiFiClientSecure();
   client = new MQTTClient(256);

  connectWifi();
  setSystemTime();
  
  certClient = new CertClient(net);
  awsClient = new AWSClient(net, client, certClient, configClient);

  SReader.begin();
  
  awsClient->connectAWS();
}

void loop() {
  Serial.println("Looping...");
  timeClient.update();

  reading = SReader.readUART();

  buildMessage(reading, timeClient.getEpochTime());
  Serial.println(jsonBuffer);
  awsClient->publishMessage(jsonBuffer);

  client->loop();
  delay(10000);
}
