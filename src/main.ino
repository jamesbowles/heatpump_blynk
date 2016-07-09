#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <MitsubishiHeatpumpIR.h>
#include <BlynkSimpleEsp8266.h>
#include <SPI.h>
#include "PrivateConfig.h"

char auth[] = BYLNK_TOKEN;
char wifi_password[] = WIFI_PASSWORD;
char wifi_ssid[] = WIFI_SSID;

int temp = 18;
int state = 0;
IRSenderBitBang irSender(5);
MitsubishiFDHeatpumpIR *heatpumpIR;

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, wifi_ssid, wifi_password);
  heatpumpIR = new MitsubishiFDHeatpumpIR();

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  //Serial.print("IP address: ");
  //Serial.println(WiFi.localIP());
}

// Process updates from the tempature slider, store it for later
BLYNK_WRITE(V1)
{
  int pinValue = param.asInt();
  temp = pinValue;
  BLYNK_PRINT.print("V1 Slider value is: ");
  BLYNK_PRINT.println(pinValue);

  if (state == 1) {
    heatpumpIR->send(irSender, state, MODE_HEAT, FAN_2, temp, VDIR_UP, HDIR_AUTO);
  }

}

// Process the desired state of the heat pump
BLYNK_WRITE(V0)
{
  int pinValue = param.asInt();
  state = pinValue;

  if (state == 1) {
    BLYNK_PRINT.print("turning on with temp ");
    BLYNK_PRINT.print(temp);
    heatpumpIR->send(irSender, state, MODE_HEAT, FAN_2, temp, VDIR_UP, HDIR_AUTO);
  } else {
    BLYNK_PRINT.print("turning off");
    heatpumpIR->send(irSender, state, MODE_HEAT, FAN_2, temp, VDIR_UP, HDIR_AUTO);
  }

}

void loop() {
  ArduinoOTA.handle();
  Blynk.run();
}
