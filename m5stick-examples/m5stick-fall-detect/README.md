# AWS IoT M5Stick Fall Detection Example

This example publishes a message to AWS IoT when a free fall is detected using the built in IMU.

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

## Free Fall Detection

Free fall is detected by calculating the square root of the sum of all the axis acceleration squared. The closer this total acceleration is to 0, the more likely a free fall is occuring.

Here we publish a message when total acceleration is below 0.10:

```c
M5.IMU.getAccelData(&accX,&accY,&accZ);
  float at = sqrt(accX*accX + accY*accY + accZ*accZ);

  // Publish a message when a fall is detected
  if(at < 0.10) {
    publishMessage();
    delay(2000);
  }
```

The published message contains a phone number and a message. This can be used with an [AWS Lambda function to send an SOS via SMS](https://aws.amazon.com/blogs/contact-center/using-amazon-pinpoint-to-send-text-messages-in-amazon-connect/).

```c
void publishMessage()
{
  StaticJsonDocument<200> doc;
  doc["sms"] = "+14804224242";
  doc["message"] = "Help! I've fallen and I can't get up!";
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}
```


Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved. SPDX-License-Identifier: MIT-0
