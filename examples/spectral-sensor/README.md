# AWS IoT Spectral Sensor

This example connects a [Sparkfun Spectral Triad (AS7265x)](https://learn.sparkfun.com/tutorials/spectral-triad-as7265x-hookup-guide/introduction) sensor to an Arduino connected to AWS IoT. This sensor examines light absorption on various wavelengths across IR, UV, and visible light. These values can be used to detect different materials.


Publish a command to the AWS IoT topic **spectral/commands** to take a sample. The classification field will be passed along to keep track of the object being evaluated.

```json
{
  "command": "takeSample",
  "classification": "banana"
}
```

Subscribe to topic **spectral/samples** to receive data samples.

Example sample message:
```json
{
  "timestamp": "Wed Sep 11 02:22:35 2019\n",
  "classification": "banana",
  "A": 110.8303,
  "B": 56.71481,
  "C": 98.04254,
  "D": 31.62038,
  "E": 55.91333,
  "F": 97.71917,
  "G": 13.80511,
  "H": 19.40129
}
```


## Additional Library needed:

- [Spectral Triad (AS7265x) Arduino Library](https://github.com/sparkfun/SparkFun_AS7265x_Arduino_Library)

## Wiring

The Spectral sensor can be connected to an ESP32 using the I2C interface. The default I2C pins may differ from board to board, consult module documentation for the correct pinout before wiring.

On the Sparkfun ESP32 Thing:

| ESP32 | AS7265x |
| ------ | -------- |
| GPIO21 | SDA |
| GPIO22 | SCL |
| GND    | GND |
| 3v3    | 3v3 |


Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved. SPDX-License-Identifier: MIT-0
