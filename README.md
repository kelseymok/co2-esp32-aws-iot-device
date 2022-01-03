# CO2 ESP32 AWS IoT Device
A repository following a curiosity adventure during the winter holidays into ESP32 C++ programning and AWS IoT devices. Based on [AWS's ESP32 IoT blog post](https://aws.amazon.com/blogs/compute/building-an-aws-iot-core-device-using-aws-serverless-and-an-esp32/)

## Prerequisites
* [Terraform (v1.1.2)](https://learn.hashicorp.com/tutorials/terraform/install-cli)
* [Arduino IDE](https://www.arduino.cc/en/software)

## Quickstart
1. Set AWS CLI credentials in the default profile
2. `cd infrastructure && terraform init`
3. `cd infrastructure && terraform apply`
4. [Set up Arduino IDE](#set-up-arduino-ide)
5. [Install ESP32 Arduino File Uploader](#install-esp32-arduino-file-uploader)
6. Go to: **Tools** > **Boards** > **Boards Manager**. Find and select ESP32 and your appropriate board.
7. Open Sketch `app.ino` under `device/app`
8. In Terminal, run `~/generate-config.sh` to generate the device config / certificates (it will prompt for your Wifi Ssid / Password). NOTE: you must have a valid AWS session.
9. Upload data **Tools** > **ESP32 Sketch Data Upload**
10. Compile code and flash device (`command + u`)

## Set up Arduino IDE
1. [Download the Arduino installer](https://www.arduino.cc/en/software) for your operating system.
2. Start the Arduino IDE
3. Under **Preferences** > **Settings** > **Additional Board Manager URLs**, add `https://dl.espressif.com/dl/package_esp32_index.json`
4. **Tools** > **Board** > **Boards Manager**, search for `esp32` and install the latest version.
5. Under Sketch > Include Library > Manage Libraries, install the following:
   1. MQTT (latest version by Joel Gaehwiler)
   2. ArduinoJson

## Install ESP32 Arduino File Uploader
1. Find the latest release of the [File Uploader](https://github.com/me-no-dev/arduino-esp32fs-plugin/releases/) and download the `.zip`
2. Create a directory in the Ardunio IDE called `tools`. On a mac: `mkdir ~/Documents/Arduino/tools`
3. Unzip the file uploader into the new tools directory. (`~/Documents/Arduino/tools/ESP32FS/tool/esp32fs.jar`)
4. Restart Arduino IDE
5. There should now be a **Tools** > **ESP32 Sketch Data Upload** option which can be run on-demand
6. All files to be uploaded should be placed at `app/data`

## References
### Reading from the CO2 Sensor
* [Reading values from a CO2 Sensor](https://forum.arduino.cc/t/problem-with-co2-sensor-mh-z19b-cannot-read-values/504873/6#msg3587557)
* [Full example](https://github.com/tobiasschuerg/MH-Z-CO2-Sensors/blob/4e868c9cee8a86066a1287c9f2b46fee7e293e93/MHZ.cpp)
* [How to use Serial2](https://github.com/G6EJD/ESP32-Using-Hardware-Serial-Ports/blob/master/ESP32_Using_Serial2.ino)

### Helpful libraries
* [HardwareSerial.cpp](https://github.com/espressif/arduino-esp32/blob/108e46716461b84fbb53814e45dc18a19a79fcdd/cores/esp32/HardwareSerial.cpp)
* [Using HardwareSerial to send/receive strings over serial](https://www.esp32.com/viewtopic.php?t=10300)

### Loading Files onto the Device
* [...for an ESP8266](https://github.com/esp8266/Arduino/issues/2470)
* [Loading files on an ESP32 using SPIFFS](https://randomnerdtutorials.com/esp32-vs-code-platformio-spiffs/)
* [SPIFFS and JSON to save configurations on an ESP8266](https://www.youtube.com/watch?v=jIOTzaeh7fs)

### Can I do this in Python?
* [Installing Micropython](https://docs.micropython.org/en/latest/esp32/quickref.html#installing-micropython)

## Troubleshooting
### Can't upload data to device because "device is busy"
Because this device runs infinitely, the ports are "busy". What's worked for me is unplugging the device from my USB port, closing Arduino IDE, opening Arduino IDE, plugging the device back in, and then attempting to do the data upload. Afterwards, compile and flash the device as you usually do.