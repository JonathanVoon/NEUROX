# NEUROX Hardware

This folder contains the hardware and wiring documentation for the complete NEUROX rehabilitation system.

The system consists of two ESP32-based controller nodes that work together to provide motion tracking, vibration feedback, temperature-controlled water therapy, and pneumatic massage.

## System Components

### Main Node

The Main Node uses an ESP32 Dev Module to control the therapy actuators and monitor water temperature.

It includes:

* ESP32 Dev Module
* 2 × DS18B20 temperature sensors
* Warm water pump
* Cold water pump
* Air pump
* Solenoid valve
* Heating pad
* Relay modules
* 12 V battery supply

### Secondary Node

The Secondary Node uses an ESP32-C3 for motion tracking and vibration feedback.

It includes:

* ESP32-C3
* 2 × MPU6050 IMU sensors
* 2 × vibration motors
* Low-voltage power supply

## Wiring Diagram

The complete wiring diagram shows the connections between the two controller nodes, sensors, actuators, relay modules, and power supplies.

![NEUROX System Wiring Diagram](fritzing/NEUROX_Wiring.png)

## Fritzing File

The editable Fritzing project is provided below:

```text
fritzing/NEUROX_Wiring.fzz
```

The `.fzz` file can be opened and edited using Fritzing.

## Note on Fritzing Components

Some of the actual components used in NEUROX are not available in the Fritzing component library.

Therefore, some components in the diagram are represented using alternative components or similar labels for visual and wiring representation.

These alternative components are used only to show the intended connections and should not be taken as the exact physical components used in the final prototype.

The actual component specifications and hardware should be referred to when building the physical system.

## Power System

The actuator components are powered from a 12 V battery supply.

The ESP32 controllers and low-voltage sensors are powered separately from their appropriate power supplies.

Relay modules are used to switch the higher-power actuator loads while allowing the ESP32 to control them using GPIO signals.

## Communication

The Main Node and Secondary Node communicate wirelessly using ESP-NOW.

The Secondary Node handles motion detection and vibration feedback, while the Main Node handles the therapy actuators and temperature monitoring.

## Hardware Documentation

This branch is intended for the overall NEUROX hardware documentation and combined wiring diagrams.

Individual firmware can be found in the following branches:

* `main-node` - Main Node firmware
* `secondary-node` - Secondary Node firmware
* `web` - NEUROX web application
