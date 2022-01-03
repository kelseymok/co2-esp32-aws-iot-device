#pragma once
#include <string>

class AppConfigClient {
  public:
    AppConfigClient(const char* s);
    
    void begin();
    std::string awsIotEndpoint;
    std::string awsIotPublishTopic;
    std::string thingName;
    std::string wifiSsid;
    std::string wifiPassword;
  private:
    const char* configPath;
};
