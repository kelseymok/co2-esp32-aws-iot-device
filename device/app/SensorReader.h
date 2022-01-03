#pragma once

#include <HardwareSerial.h>

class SensorReader {
  
  public: 
    int readUART();
    void begin();
  private:
    const int8_t RX = 17;
    const int8_t TX = 16;
    byte calculateChecksum(byte* b);
    void requestData();
    void processResponse(byte* resp);
    int calculateCo2(byte high, byte low);
    void validateChecksum(byte actual, byte expected);
};

void SensorReader::begin() {
    Serial2.begin(9600, SERIAL_8N1, TX, RX);
}

int SensorReader::readUART() {
  byte response[9];

  Serial.println("Reading...");
  requestData();
  memset(response, 0, 9);
  processResponse(response);
  
  byte high = response[2];
  byte low = response[3];
  byte checksum = response[8];

  return calculateCo2(high, low);
}


void SensorReader::requestData() {
  byte cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};

  Serial.println("Sending request...");
  Serial2.write(cmd, 9);
}

void SensorReader::processResponse(byte* resp) {
    if (Serial2.available() > 0) {
      Serial2.read(resp, 9);
    }
}

int SensorReader::calculateCo2(byte high, byte low) {
  Serial.println("Calculating CO2 PPM");
  return 256 * (int)high + low;
}

void SensorReader::validateChecksum(byte expected, byte actual) {
  if (actual != expected) {
    Serial.println("Checksum failed");
    Serial.print("Received: ");
    Serial.println(actual, HEX);
    Serial.print("Expected: ");
    Serial.println(expected, HEX);
    Serial2.flush();
  }
}


byte SensorReader::calculateChecksum(byte* b) {
  byte i;
  unsigned char checksum = 0;
  for (i = 1; i < 8; i++) {
    checksum += b[i];
  }
  checksum = 0xff - checksum;
  checksum += 1;
  return checksum;
}
