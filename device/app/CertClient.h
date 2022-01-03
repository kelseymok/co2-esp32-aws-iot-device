#pragma once
#include <WiFiClientSecure.h>


class CertClient {
  public:
    CertClient(WiFiClientSecure* client);
    void begin();
    void setCA(String filename);
    void setDeviceCertificate(String filename);
    void setDevicePrivateKey(String filename);
  private:
    WiFiClientSecure* client;
};
