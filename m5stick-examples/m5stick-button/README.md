# AWS IoT M5Stick Button Example

This example publishes a message to AWS IoT when the large button of the M5stick is pressed.  

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

The **publishMessage()** function creates a JSON object with the current time in milliseconds and a message string. It then publishes this JSON object to the topic **esp32/pub**.

```c
void publishMessage()
{
  StaticJsonDocument<200> doc;
  doc["time"] = millis();
  doc["message"] = "Hello World";
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}
```

## Button handler

Inside the main loop(), we read the current button value and compare it to the last. If it has been pressed, we publish, otherwise log the state and do nothing.

```c
// read the value of BUTTON
cur_value = digitalRead(button);

if(cur_value != last_value){
  if(cur_value==0){
    Serial.println("Button pressed");
    publishMessage();
    Serial.println("Publishing");
  }
  else{
    Serial.println("Button Status: released");
    Serial.println("value:  1");
  }
  last_value = cur_value;
}
```

At the top of the sketch we create some variables including the pin number for the button, 37.

```c
const int button = 37;
int last_value = 0;
int cur_value = 0;
```

In the setup() function we set pin 39 to be an input with this line.

```c
pinMode(button, INPUT);
```

Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved. SPDX-License-Identifier: MIT-0
