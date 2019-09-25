/*
  Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
  Permission is hereby granted, free of charge, to any person obtaining a copy of this
  software and associated documentation files (the "Software"), to deal in the Software
  without restriction, including without limitation the rights to use, copy, modify,
  merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include "secrets.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include "time.h"
#include <Wire.h>
#include "SparkFun_AS7265X.h"
AS7265X sensor;

// The MQTT topics that this device should publish/subscribe
#define AWS_IOT_PUBLISH_TOPIC   "spectral/samples"
#define AWS_IOT_SUBSCRIBE_TOPIC "spectral/commands"

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(256);

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

void connectAWS() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  // Init time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.begin(AWS_IOT_ENDPOINT, 8883, net);

  // Create a message handler
  client.onMessage(messageHandler);

  Serial.print("Connecting to AWS IOT");

  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }

  if(!client.connected()){
    Serial.println("AWS IoT Timeout!");
    return;
  }

  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("AWS IoT Connected!");
}

void messageHandler(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

  // deserialize json
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);

  String classification = doc["classification"];

  // handle command
  if(doc["command"] == "takeSample"){
    // read and publish sample
    takeSample(classification);
  }
}

void startSpectral() {
  if(sensor.begin() == false)
  {
    Serial.println("Sensor does not appear to be connected. Please check wiring. Freezing...");
    while(1);
  }
  sensor.disableIndicator(); //Turn off the blue status LED
}

void takeSample(String classification) {
  Serial.println("Sampling the " + classification);

  sensor.takeMeasurementsWithBulb();
  StaticJsonDocument<200> doc;
  doc["timestamp"] = getTime();
  doc["classification"] = classification;

  doc["A"] = sensor.getCalibratedA();
  doc["B"] = sensor.getCalibratedB();
  doc["C"] = sensor.getCalibratedC();
  doc["D"] = sensor.getCalibratedD();
  doc["E"] = sensor.getCalibratedE();
  doc["F"] = sensor.getCalibratedF();
  doc["G"] = sensor.getCalibratedG();
  doc["H"] = sensor.getCalibratedH();
  doc["I"] = sensor.getCalibratedI();
  doc["J"] = sensor.getCalibratedJ();
  doc["K"] = sensor.getCalibratedK();
  doc["L"] = sensor.getCalibratedL();
  doc["R"] = sensor.getCalibratedR();
  doc["S"] = sensor.getCalibratedS();
  doc["T"] = sensor.getCalibratedT();
  doc["U"] = sensor.getCalibratedU();
  doc["V"] = sensor.getCalibratedV();
  doc["W"] = sensor.getCalibratedW();

  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
  Serial.println(jsonBuffer);

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

String getTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    return "Failed to obtain time";
  }
  return asctime(&timeinfo);
}

void setup() {
  Serial.begin(9600);

  startSpectral();
  connectAWS();
}

void loop() {
  client.loop();
  delay(1000);
}
