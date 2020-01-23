# AWS IoT M5Stick ENV Example

This example publishes sensor values from the M5Stick ENV sensor hat to AWS IoT. It requires a few specific Adafruit sensor libraries listed below.

## Required Arduino Libraries:

- [MQTT](https://github.com/256dpi/arduino-mqtt)
- [ArduinoJson](https://arduinojson.org/)
- [Adafruit Unified Sensor](https://github.com/adafruit/Adafruit_Sensor)
- [Adafruit BMP280](https://github.com/adafruit/Adafruit_BMP280_Library)


You will need to [register an AWS IoT device](https://docs.aws.amazon.com/iot/latest/developerguide/register-device.html) and create credentials.

## Required Parameters in secrets.h:

- **WIFI_SSID**: The name of the Wi-Fi network to connect to. Should not include special characters or spaces.
- **WIFI_PASSWORD**: The password of the Wi-Fi network to connect to.
- **THINGNAME**: Must match the name of the device in the AWS IoT console.
- **AWS_IOT_ENDPOINT**: The endpoint associated with your AWS IoT Core account.
- **AWS_CERT_CA**: [Amazon Root CA 1](https://www.amazontrust.com/repository/AmazonRootCA1.pem)  
- **AWS_CERT_CRT**: AWS IoT Thing Certificate
- **AWS_CERT_PRIVATE**: AWS IoT Thing Private Key


Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved. SPDX-License-Identifier: MIT-0
