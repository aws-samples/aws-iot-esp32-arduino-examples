# AWS IoT + esp32

This project contains several Arduino examples for [AWS IoT](https://aws.amazon.com/iot/) projects using ESP32 Wi-Fi+BLE based development modules.

Arduino support for ESP32 modules can be found [here](https://github.com/espressif/arduino-esp32).

For each example you will need to [register an AWS IoT device](https://docs.aws.amazon.com/iot/latest/developerguide/register-device.html) and create credentials.

Each example has a "secrets.h" file where Wi-Fi and device credentials are to be stored.

## Required Parameters:

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

## Examples:

- **basic-pubsub**: Connects to AWS IoT. Demonstrates subscribing to and publishing to topics. Works great with the [lambda-iot-rule](https://serverlessrepo.aws.amazon.com/applications/arn:aws:serverlessrepo:us-east-2:826492788183:applications~lambda-iot-rule) serverless application.
- **thermal-printer**: Connects to AWS IoT and subscribes to a topic. Prints messages out to a physical receipt printer.
- **spectral-sensor**: This example connects a [Sparkfun Spectral Triad (AS7265x)](https://learn.sparkfun.com/tutorials/spectral-triad-as7265x-hookup-guide/introduction) sensor to an Arduino connected to AWS IoT. This sensor examines light absorption on various wavelengths across IR, UV, and visible light. These values can be used to detect different materials.

## M5Stick Examples:

- **m5stick-lcd**: Connect an [M5Stick](https://m5stack.com/products/stick-c) to AWS IoT and display messages on its built in LCD.
- **m5stick-env**: Publish magnetic compass heading, temperature, barometric pressure, and humidity to AWS IoT using an [M5Stick with the ENV hat module](https://m5stack.com/collections/m5-core/products/m5stickc-development-kit-with-hat).

## Sample AWS IoT Policy

```json
{
  "Version": "2012-10-17",
  "Statement": [
    {
      "Effect": "Allow",
      "Action": "iot:Connect",
      "Resource": "arn:aws:iot:REGION:ACCOUNT_ID:client/THINGNAME"
    },
    {
      "Effect": "Allow",
      "Action": "iot:Subscribe",
      "Resource": "arn:aws:iot:REGION:ACCOUNT_ID:topicfilter/esp32/sub"
    },
	{
      "Effect": "Allow",
      "Action": "iot:Receive",
      "Resource": "arn:aws:iot:REGION:ACCOUNT_ID:topic/esp32/sub"
    },
    {
      "Effect": "Allow",
      "Action": "iot:Publish",
      "Resource": "arn:aws:iot:REGION:ACCOUNT_ID:topic/esp32/pub"
    }
  ]
}
```

Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved. SPDX-License-Identifier: MIT-0
