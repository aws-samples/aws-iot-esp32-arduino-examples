# AWS IoT + M5StickC

![alt-text](https://pbs.twimg.com/media/ENUSd1jUYAAepFI?format=jpg&name=4096x4096)

## What is this?

Included in this folder are several example Arduino projects for running AWS IoT on the M5Stick ESP32.

![alt-text](https://d2908q01vomqb2.cloudfront.net/1b6453892473a467d07372d45eb05abc2031647a/2020/01/15/Screen-Shot-2020-01-08-at-3.20.35-PM.png)

The following instructions show how to get started programming and configuring your device. It also walks through deploying an AWS Serverless application using the AWS Serverless Application Repository (SAR). The application will create two AWS Lambda functions for both consuming and publishing messages to and from the device.

This guide is adapted from the [Building an AWS IoT Core device using AWS Serverless and an ESP32](https://aws.amazon.com/blogs/compute/building-an-aws-iot-core-device-using-aws-serverless-and-an-esp32/) guide from the AWS Compute Blog.

## Downloads

- [CP210 Serial Driver](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers)
- [Arduino installer](https://arduino.cc)

## Required Arduino Libraries:

This guide will install these using the Arduino library manager, but can also be downloaded and installed directly.

- [MQTT](https://github.com/256dpi/arduino-mqtt)
- [ArduinoJson](https://arduinojson.org/)
- [M5StickC](https://github.com/m5stack/M5StickC)

## Installing and configuring the Arduino IDE

The Arduino IDE is an open-source development environment for programming microcontrollers. It can support a continuously growing number of platforms including most ESP32-based modules. It must be installed along with the ESP32 board definitions, MQTT library, ArduinoJson library, and the M5Stick library.

1. Download and install the [CP210 Serial Driver](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers) for your operating system.
2. Download the [Arduino installer](https://arduino.cc) for the desired operating system.
3. Start Arduino and open the **Preferences** window.
4. For **Additional Board Manager URLs**, add
https://dl.espressif.com/dl/package_esp32_index.json.
5. Choose **Tools**, **Board**, **Boards Manager**.
6. Search **esp32** and install the latest version.
7. Choose **Sketch**, **Include Library**, **Manage Libraries**.
8. Search **MQTT**, and install the latest version by **Joel Gaehwiler**.
9. Repeat the library installation process for **ArduinoJson** and **M5StickC**

The Arduino IDE is now installed and configured with all the board definitions and libraries needed for this walkthrough.

## Creating AWS IoT device credentials

To communicate with the ESP32 device, it must connect to AWS IoT Core with device credentials. You must also specify the topics it has permissions to publish and subscribe on.

1. In the AWS IoT console, choose **Register a new thing**, **Create a single thing**.
2. Name the new thing **MyNewESP32**. Leave the remaining fields set to their defaults. Choose Next.
3. Choose **Create certificate**. Only the *thing cert*, *private key*, and [Amazon Root CA 1](https://www.amazontrust.com/repository/AmazonRootCA1.pem) downloads are necessary for the ESP32 to connect. Download and save them somewhere secure, as they are used when programming the ESP32 device.
4. Choose **Activate**, **Attach a policy**.
5. Skip adding a policy, and choose **Register Thing**.
6. In the AWS IoT console side menu, choose **Secure**, **Policies**, **Create a policy**.
7. Name the policy **Esp32Policy**. Choose the **Advanced tab**.
8. Paste in the following policy template.

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

9. Replace **REGION** with the matching AWS Region you’re currently operating in. This can be found on the top right corner of the AWS console window.
10. Replace **ACCOUNT_ID** with your own, which can be found in Account Settings.
11. Choose **Create**.
12. In the AWS IoT console, choose **Secure**, **Certification**. Select the one created for your device and choose **Actions**, **Attach policy**.
13. Choose **Esp32Policy**, Attach.

Your AWS IoT device is now configured to have permission to connect to AWS IoT Core. It can also publish to the topic **esp32/pub** and subscribe to the topic **esp32/sub**. For more information on securing devices, see AWS IoT Policies.

## Flash the M5StickC

1. Download [this GitHub project](https://github.com/aws-samples/aws-iot-esp32-arduino-examples) and open **m5stick-examples/m5stick-lcd** in the Arduino IDE.
2. Fill in the following required parameters in the **secrets.h** tab. Use the credentials made in the previous section.

  - **WIFI_SSID**: The name of the Wi-Fi network to connect to. Should not include special characters or spaces.
  - **WIFI_PASSWORD**: The password of the Wi-Fi network to connect to.
  - **THINGNAME**: Must match the name of the device in the AWS IoT console.
  - **AWS_IOT_ENDPOINT**: The endpoint associated with your AWS IoT Core account.
  - **AWS_CERT_CA**: [Amazon Root CA 1](https://www.amazontrust.com/repository/AmazonRootCA1.pem)  
  - **AWS_CERT_CRT**: AWS IoT Thing Certificate
  - **AWS_CERT_PRIVATE**: AWS IoT Thing Private Key


3. Choose **Tools**, **Board**, and then select the matching type of ESP32 module. In this case, the **M5Stick**.
4. Choose **Tools**, **Port**, and then select the matching port for your device.
5. Choose **Upload**. Arduino reads *Done uploading* when the upload is successful.
6. Choose the magnifying lens icon to open the **Serial Monitor**. Set the baud rate to **9600**.

Your device will start logging when it tries to connect to Wi-Fi and then AWS IoT in the Serial Monitor.

## Deploying the lambda-iot-rule AWS SAR application

![alt-text](https://d2908q01vomqb2.cloudfront.net/1b6453892473a467d07372d45eb05abc2031647a/2020/01/15/Screen-Shot-2020-01-08-at-3.20.35-PM.png)


Now that an ESP32 device has been connected to AWS IoT, the following steps walk through deploying an AWS Serverless Application Repository application. This is a base for building serverless integration with a physical device.

1. On the [lambda-iot-rule](https://serverlessrepo.aws.amazon.com/applications/arn:aws:serverlessrepo:us-east-2:826492788183:applications~lambda-iot-rule) AWS Serverless Application Repository application page, make sure that the Region is the same as the AWS IoT device.
2. Choose **Deploy**.
3. Under **Application settings**, for **PublishTopic**, enter **esp32/sub**. This is the topic to which the ESP32 device is subscribed. It receives messages published to this topic. Likewise, set **SubscribeTopic** to **esp32/pub**, the topic on which the device publishes.
4. Choose **Deploy**.
5. When creation of the application is complete, choose **Test app** to navigate to the application page. Keep this page open for the next section.

## Monitoring and testing

At this stage, two Lambda functions, a DynamoDB table, and an [AWS IoT rule](https://docs.aws.amazon.com/iot/latest/developerguide/iot-rules.html) have been deployed. The IoT rule forwards messages on topic esp32/pub to TopicSubscriber, a Lambda function, which inserts the messages on to the DynamoDB table.

![alt-text](https://d2908q01vomqb2.cloudfront.net/1b6453892473a467d07372d45eb05abc2031647a/2020/01/02/Picture7.png)

1. On the application page, under Resources, choose MyTable. This is the DynamoDB table that the TopicSubscriber Lambda function updates.
2. Choose Items. If the ESP32 device is still active and connected, messages that it has published appear here.
The TopicPublisher Lambda function is invoked by the API Gateway endpoint and publishes to the AWS IoT topic esp32/sub.

![alt-text](https://d2908q01vomqb2.cloudfront.net/1b6453892473a467d07372d45eb05abc2031647a/2020/01/02/Picture8.png)

1. On the application page, find the Application endpoint.

2. To test that the TopicPublisher function is working, enter the following into a terminal or command-line utility, replacing ENDPOINT with the URL from above.

```bash
curl -d '{"text":"Hello world!"}' -H "Content-Type: application/json" -X POST https://ENDPOINT/publish
```

Upon success, the request returns a copy of the message.

![alt-text](https://d2908q01vomqb2.cloudfront.net/1b6453892473a467d07372d45eb05abc2031647a/2020/01/02/Picture9.png)

Back in the Serial Monitor, the message published to the topic esp32/sub prints out.

![alt-text](https://d2908q01vomqb2.cloudfront.net/1b6453892473a467d07372d45eb05abc2031647a/2020/01/02/Picture10.png)

## M5Stick Examples:

- [m5stick-lcd](https://github.com/aws-samples/aws-iot-esp32-arduino-examples/tree/master/m5stick-examples/m5stick-lcd): Connect an [M5Stick](https://m5stack.com/products/stick-c) to AWS IoT and display messages on its built in LCD.
- [m5stick-button](https://github.com/aws-samples/aws-iot-esp32-arduino-examples/tree/master/m5stick-examples/m5stick-button): Publish an AWS IoT message on a button press.
- [m5stick-env](https://github.com/aws-samples/aws-iot-esp32-arduino-examples/tree/master/m5stick-examples/m5stick-env): Publish magnetic compass heading, temperature, barometric pressure, and humidity to AWS IoT using an [M5Stick with the ENV hat module](https://m5stack.com/collections/m5-core/products/m5stickc-development-kit-with-hat).
- [m5stick-fall-detect](https://github.com/aws-samples/aws-iot-esp32-arduino-examples/tree/master/m5stick-examples/m5stick-fall-detect): Publish an AWS IoT message when a free fall is detected using the built in IMU.


Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved. SPDX-License-Identifier: MIT-0
