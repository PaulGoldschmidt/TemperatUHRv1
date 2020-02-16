/*************************************************************

  TemperatUHR - Main Microcontroller Code
  In order to configure the program right, you have to change
  three things: Put thr right access-token into the space.
  Change the SSID and PASSWORD space to the right credentials
  for your WiFi.

  Code by Paul Goldschmidt, Febuary 2020.

 *************************************************************/
#define BLYNK_PRINT Serial //Debug console

#include <ESP8266WiFi.h> //all the libs are imported here - please double check that you have them installed.
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#define ONE_WIRE_BUS 14 //the sensor is connected to pin #14
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YOUR_ACCESS_TOKEN";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "SSID";
char pass[] = "PASSWORD";

float metersToSensor, notificationTemp = 35, temperaturePerSec; //defining some global variables.
WidgetLED led1(V2); //an virtual LED is in the app "connected" to V2.

void setup()
{
  pinMode(12, OUTPUT); //the RGB-LED needs its pins initialised.
  pinMode(13, OUTPUT);
  pinMode(15, OUTPUT);
  digitalWrite(12, LOW); //red || the pins of the LED are put on High, so the LED won't light up..
  digitalWrite(13, HIGH); // Green
  digitalWrite(15, HIGH); //Blue
  WiFi.mode(WIFI_STA);
  // Debug console
  Serial.begin(9600);
  Serial.println("TemperatUHR Startup");
  sensors.begin(); //lets begin the communication with the senosor
  Blynk.begin(auth, ssid, pass);
  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);
  ArduinoOTA.setHostname("TemperatUHR"); //Down below is all the OTA Stuff
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }
    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  led1.off(); //check that the LED in the app is turned off.
}

BLYNK_WRITE(1) { //BLYNK.WRITE can now give the notification tempemperature back
  notificationTemp = param.asInt();
}

BLYNK_WRITE(3) { //BLYNK.WRITE can now give the way to TemperatUHR back
  metersToSensor = param.asInt();
}

void tempchange() { //this functions calcuates the temp difference that happens in 10 seconds.
  sensors.requestTemperatures();
  float temperaturestart = sensors.getTempCByIndex(0); // get the temperature on start
  delay(1000); //one second delay;
  sensors.requestTemperatures();
  float temperatureend = sensors.getTempCByIndex(0); // get the temperature after one second
  temperaturePerSec = temperatureend - temperaturestart; //calc the difference
}

void loop()
{
  digitalWrite(13, HIGH); //turn green LED off
  boolean CooldownMode; //in case the temperature of the liquid is higher than the set temperature
  ArduinoOTA.handle(); //handle OTA-Updates
  sensors.requestTemperatures(); //request temp sensor data
  float temperature = sensors.getTempCByIndex(0); //store it in the float temperature
  if (temperature == -127) { //this function will go into work when the sensor isnt connected.
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
    delay(4500);
    digitalWrite(12, HIGH);
    digitalWrite(13, HIGH);
    while (true) {
      ArduinoOTA.handle();
      delay(250);
    }
  }
  delay(50);
  tempchange(); //calls the function to calc the temp. rise
  digitalWrite(13, LOW); //turns LED on
  BLYNK_WRITE(1); // reading the notification Temperature from App
  BLYNK_WRITE(3); // way from user to TemperatUHR
  float tempDelta; //calc the temp. delta for the user to display.
  if (temperature > notificationTemp + 20) { //is the substance cooling down or not?
    CooldownMode = true;
    tempDelta = temperature - notificationTemp;
  }
  else {
    CooldownMode = false;
    tempDelta = notificationTemp - temperature;
  }
  Blynk.virtualWrite(V4, tempDelta); //push the temp. delta to the app
  float tempreached = tempDelta / temperaturePerSec; // i assume the user walks with v = 1m/s
  Blynk.virtualWrite(V5, tempreached);
  Blynk.virtualWrite(V0, temperature); //push all the other data to blynk
  Serial.print("Notification Temperature: ");
  Serial.println(notificationTemp);
  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("Delta: ");
  Serial.println(tempDelta);
  Serial.print("CooldownMode: ");
  Serial.println(CooldownMode);
  Serial.print("Temp per Sec: ");
  Serial.println(temperaturePerSec); //serial debug
  sensors.requestTemperatures(); //reget the sensor data
  temperature = sensors.getTempCByIndex(0); // rewrite temperature
  if (CooldownMode == false) { //has the temp. reached yet?
    if (temperature >= notificationTemp - 5) {
      Blynk.notify("Temperatur erreicht!");
      led1.on();
      delay(50000); //wait 50s.
    }
    else {
      led1.off(); //the LED stays off.
    }
  }
  else {
    if (notificationTemp >= temperature + 5) {
      Blynk.notify("Temperatur erreicht!");
      led1.on();
      delay(50000); //wait 50s.
    }
    else {
      led1.off(); //the LED stays off.
    }
  }
  Blynk.run(); //run the blynk update process.
}
