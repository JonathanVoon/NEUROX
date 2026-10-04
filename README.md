# NEUROX Secondary Node

The Secondary Node is responsible for motion tracking and vibration feedback in the NEUROX rehabilitation system.

It uses an ESP32-C3 to read two MPU6050 motion sensors and control two vibration motors.

## Functions

The Secondary Node:

* Monitors movement using 2 × MPU6050 IMU sensors
* Detects when the user remains still
* Controls 2 × vibration motors
* Connects to Wi-Fi
* Sends and receives data using ESP-NOW
* Sends rehabilitation data to Firebase

## Hardware

| Component           | Purpose                         |
| ------------------- | ------------------------------- |
| ESP32-C3            | Secondary controller            |
| MPU6050 × 2         | Detect movement and orientation |
| Vibration motor × 2 | Provide vibration feedback      |
| Wi-Fi               | Connect to Firebase             |
| ESP-NOW             | Wireless communication          |

## Pin Configuration

| Component         | ESP32-C3 GPIO |
| ----------------- | ------------: |
| MPU6050 SDA       |        GPIO 4 |
| MPU6050 SCL       |        GPIO 5 |
| Vibration Motor 1 |        GPIO 6 |
| Vibration Motor 2 |        GPIO 7 |

Both MPU6050 sensors use the same I2C bus.

| Sensor    | I2C Address |
| --------- | ----------- |
| MPU6050 1 | `0x68`      |
| MPU6050 2 | `0x69`      |

The two sensors use different I2C addresses so they can operate on the same bus.

## Motion Detection

The MPU6050 sensors are used to detect movement and stillness.

The system monitors the gyroscope data and checks whether the user remains still for a specific period.

The current stillness detection time is approximately:

```text
2 seconds
```

When the required stillness condition is detected, the corresponding vibration motor is activated.

## Vibration Feedback

Two vibration motors are used to provide feedback to the user.

* MPU6050 1 → Vibration Motor 1
* MPU6050 2 → Vibration Motor 2

The vibration motor remains active while the required stillness condition is detected and turns off when movement is detected.

## Calibration

The MPU6050 sensors perform a gyroscope calibration during startup.

The sensors should remain still during the calibration process to obtain a more accurate reference value.

## Firebase

The Secondary Node can connect to Firebase to store rehabilitation and vibration status data.

The Firebase and Wi-Fi credentials are stored locally in:

```text
secrets.h
```

The `secrets.h` file must not be uploaded to GitHub.

A template can be provided using:

```text
secrets.example.h
```

## Project Structure

```text
secondary_node/
├── secondary_node.ino
├── secrets.h
└── README.md
```

`secrets.h` is for local use only and is ignored by Git.

## Purpose

The Secondary Node provides motion tracking and vibration feedback for the NEUROX rehabilitation system. It allows the system to monitor user movement and provide physical feedback during rehabilitation exercises.
