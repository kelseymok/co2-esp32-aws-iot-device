#include "AppConfigClient.h"
#include "FS.h"
#include "SPIFFS.h"
#include <ArduinoJson.h>
#include <HardwareSerial.h>

AppConfigClient::AppConfigClient(const char* p)
  : configPath{p}
{
}

void AppConfigClient::begin() {
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  File file = SPIFFS.open("/config.json", "r");
  if (!file)
    Serial.println("Failed to file");
  else
    Serial.println("Successfully opened file");

  std::string contents(file.size(), ' ');
  file.readBytes(&contents[0], contents.size());
  Serial.println(contents.c_str());


  StaticJsonDocument<1024> json;
  deserializeJson(json, contents);

  std::string wifiSsidStr = json["wifi_ssid"];
  std::string wifiPasswordStr = json["wifi_password"];
  std::string awsIotEndpointStr = json["aws_iot_endpoint"];
  std::string thingNameStr = json["thing_name"];
  std::string awsIotPublishTopicStr = json["aws_iot_publish_topic"];

  wifiSsid = wifiSsidStr;
  wifiPassword = wifiPasswordStr;
  awsIotEndpoint = awsIotEndpointStr;
  thingName = thingNameStr;
  awsIotPublishTopic = awsIotPublishTopicStr;
}
