/*
 * This is the sourcecode for the TemperatUHR, a Project by Pul Goldschmidt.
 * Requirements:
 * - Working hardware, schematics and other relevant stuff can be found unter "hardware"
 * - The following libaries:
 * OneWire.h
 * DallasTemperature.h
 * 
 * - You'll need the ESP Board-Libary
 * - You have to select "Board: LOLIN(WEMOS) D1 R2 & mini" in the tools-section
 * - you have to pick the right port in the same section
 */

//firstly, lets import some libaries:
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>

const char* ssid     = "SSID";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "PASSWORD";     // The password of the Wi-Fi network

#define ONE_WIRE_BUS 14 //This is the pin on witch the temperature senor is connected.

// Setup a oneWire instance to communicate with any OneWire devices 
OneWire oneWire(ONE_WIRE_BUS); 

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.println("Hi.");
  pinMode(12, OUTPUT); //the RGB-LED needs its pins initialised.
  pinMode(13, OUTPUT);
  pinMode(15, OUTPUT);
  digitalWrite(12, HIGH); //the pins of the LED are put on High, so the LED won't light up..
  digitalWrite(13, HIGH);
  digitalWrite(15, HIGH);
  Serial.begin(57600);
  sensors.begin();
  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
}

void loop() {
  // put your main code here, to run repeatedly:

}
