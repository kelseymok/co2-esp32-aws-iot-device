#include <HardwareSerial.h>
#include "SensorReader.h"

void SensorReader::begin() {
    Serial2.begin(9600, SERIAL_8N1, TX, RX);
}

int SensorReader::readUART() {
  uint8_t response[9];

  Serial.println("Reading...");
  requestData();
  memset(response, 0, 9);
  processResponse(response);
  
  uint8_t high = response[2];
  uint8_t low = response[3];
  uint8_t checksum = response[8];

  return calculateCo2(high, low);
}


void SensorReader::requestData() {
  uint8_t cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};

  Serial.println("Sending request...");
  Serial2.write(cmd, 9);
}

void SensorReader::processResponse(uint8_t* resp) {
    if (Serial2.available() > 0) {
      Serial2.read(resp, 9);
    }
}

int SensorReader::calculateCo2(uint8_t high, uint8_t low) {
  Serial.println("Calculating CO2 PPM");
  return 256 * (int)high + low;
}

void SensorReader::validateChecksum(uint8_t expected, uint8_t actual) {
  if (actual != expected) {
    Serial.println("Checksum failed");
    Serial.print("Received: ");
    Serial.println(actual, HEX);
    Serial.print("Expected: ");
    Serial.println(expected, HEX);
    Serial2.flush();
  }
}


uint8_t SensorReader::calculateChecksum(uint8_t* b) {
  uint8_t i;
  unsigned char checksum = 0;
  for (i = 1; i < 8; i++) {
    checksum += b[i];
  }
  checksum = 0xff - checksum;
  checksum += 1;
  return checksum;
}
