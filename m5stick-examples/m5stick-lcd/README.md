# AWS IoT M5Stick LCD Example


You will need to [register an AWS IoT device](https://docs.aws.amazon.com/iot/latest/developerguide/register-device.html) and create credentials.

## Required Parameters in secrets.h:

- **WIFI_SSID**: The name of the Wi-Fi network to connect to. Should not include special characters or spaces.
- **WIFI_PASSWORD**: The password of the Wi-Fi network to connect to.
- **THINGNAME**: Must match the name of the device in the AWS IoT console.
- **AWS_IOT_ENDPOINT**: The endpoint associated with your AWS IoT Core account.
- **AWS_CERT_CA**: [Amazon Root CA 1](https://www.amazontrust.com/repository/AmazonRootCA1.pem)  
- **AWS_CERT_CRT**: AWS IoT Thing Certificate
- **AWS_CERT_PRIVATE**: AWS IoT Thing Private Key

## Required Arduino Libraries:

- [MQTT](https://github.com/256dpi/arduino-mqtt)
- [ArduinoJson](https://arduinojson.org/)
- [M5StickC](https://github.com/m5stack/M5StickC)


## Setting Topics

The topics are set at the top of the sketch. When changing or adding topics, remember to add permissions in the device policy.

```c
// The MQTT topics that this device should publish/subscribe
#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"
```

## Publishing Messages

The **publishMessage()** function creates a JSON object with the current time in milliseconds and the analog value of pin A0 on the device. It then publishes this JSON object to the topic **esp32/pub**.

```c
void publishMessage()
{
  StaticJsonDocument<200> doc;
  doc["time"] = millis();
  doc["sensor_a0"] = analogRead(0);
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}
```

## Subscribing to TopicSubscriber

Topic subscriptions can be added within the **connectAWS()** function by adding another line similar to the following.

```c
// Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("AWS IoT Connected!");

```

## Handling Messages

The **messageHandler()** function prints out the topic and payload of any message from a subscribed topic. In the LCD example it is also printed on the M5Stick LCD. To see all the ways to parse JSON messages in Arduino, see the [deserializeJson()](https://arduinojson.org/v6/api/json/deserializejson/#example) example.

```c
void messageHandler(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char* message = doc["message"];

  M5.Lcd.fillScreen(TFT_GREY);
  M5.Lcd.setCursor(0, 0, 2);
  M5.Lcd.setTextColor(TFT_BLUE);
  M5.Lcd.setTextFont(2);
  M5.Lcd.println(payload);
}
```

Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved. SPDX-License-Identifier: MIT-0
