#include "AWSClient.h"
#include "CertClient.h"
#include "AppConfigClient.h"

#include <WiFiClientSecure.h>
#include <MQTTClient.h>

AWSClient::AWSClient(WiFiClientSecure* w, MQTTClient* m, CertClient* c, AppConfigClient* conf)
  : wifiClient{w}, mqttClient{m}, certClient{c}, configClient{conf}
{
}

void AWSClient::connectAWS() {
  certClient->begin();
  certClient->setCA("/ca.crt");
  certClient->setDeviceCertificate("/device.pem.crt");
  certClient->setDevicePrivateKey("/private-key.pem");
  Serial.println("configured certClient");

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  Serial.println("Connecting to MQTT broker");
  mqttClient->begin(configClient->awsIotEndpoint.c_str(), 8883, *wifiClient);
  Serial.println("Connected to MQTT broker");

  Serial.print("Connecting to AWS IOT");
  while (!mqttClient->connect(configClient->thingName.c_str())) {
    Serial.print(".");
    delay(100);
  }

  if(!mqttClient->connected()){
    Serial.println("AWS IoT Timeout! Could not connect to MQTT Client.");
    return;
  }


  Serial.println("AWS IoT Connected!");
}

void AWSClient::publishMessage(char* jsonBuffer) {
  mqttClient->publish(configClient->awsIotPublishTopic.c_str(), jsonBuffer);
}
