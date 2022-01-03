#pragma once

#include <HardwareSerial.h>

class SensorReader {
  
  public: 
    int readUART();
    void begin();
  private:
    const int8_t RX = 17;
    const int8_t TX = 16;
    uint8_t calculateChecksum(uint8_t* b);
    void requestData();
    void processResponse(uint8_t* resp);
    int calculateCo2(uint8_t high, uint8_t low);
    void validateChecksum(uint8_t actual, uint8_t expected);
};
