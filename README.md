# heatpump_blynk

A internet connected remote control for my Mitsubishi heatpump using ESP8266 and Blynk

## Dependencies
* [arduino-heatpumpir](https://github.com/ToniA/arduino-heatpumpir)
* [Blynk](https://github.com/blynkkk/blynk-library)
* [ESP8266 Arduino](https://github.com/esp8266/Arduino) - I think this is included with Platformio

## Setup
1. Using [Platfomio](http://platformio.org/) clone and import the project.
1. Install the dependencies
    * `platformio lib install 290`
    * `platformio lib install 415`
1. comment out `upload_port` in [`platformio.ini`](https://github.com/jamesbowles/heatpump_blynk/blob/master/platformio.ini)
1. Upload the sketch via serial

### Enable OTA updates

1. Enter the local IP address of the ESP8266 as the `upload_port` in [`platformio.ini`](https://github.com/jamesbowles/heatpump_blynk/blob/master/platformio.ini)
1. The sketch will now upload via WiFi
