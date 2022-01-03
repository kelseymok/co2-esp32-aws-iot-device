#include <WiFiClientSecure.h>
#include "CertClient.h"
#include "FS.h"
#include "SPIFFS.h"

#include <HardwareSerial.h>


CertClient::CertClient(WiFiClientSecure* c)
  : client{c}
{
}

void CertClient::begin() {
  if(!SPIFFS.begin(true)){
    Serial.println("An error has occurred while mounting SPIFFS");
  }
}

// Update with something like this? https://github.com/espressif/arduino-esp32/issues/3248#issuecomment-793522986
void CertClient::setCA(String filename) {
    Serial.println("Opening ca file");
    File file = SPIFFS.open(filename, "r");
    if (!file)
      Serial.println("Failed to open ca file");
    else
      Serial.println("Successfully opened ca file");
    client->loadCACert(file, file.size());
}

void CertClient::setDeviceCertificate(String filename) {
    Serial.println("Opening certificate file");
     File file = SPIFFS.open(filename, "r");
    if (!file)
      Serial.println("Failed to open certificate file");
    else
      Serial.println("Successfully opened certificate file");
    client->loadCertificate(file, file.size());
 
}

void CertClient::setDevicePrivateKey(String filename) {
  Serial.println("Opening private key file");
    File file = SPIFFS.open(filename, "r");
    if (!file)
      Serial.println("Failed to open private file");
    else
      Serial.println("Successfully opened private file");
    client->loadPrivateKey(file, file.size());
}
