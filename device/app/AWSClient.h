#pragma once

class WiFiClientSecure;
class MQTTClient;
class CertClient;
class AppConfigClient;

class AWSClient {
  public:
    AWSClient(WiFiClientSecure* w, MQTTClient* m, CertClient* c, AppConfigClient* conf);

    void connectAWS();
    void publishMessage(char* message);

  private:
    WiFiClientSecure* wifiClient;
    MQTTClient* mqttClient;
    CertClient* certClient;
    AppConfigClient* configClient;
};
