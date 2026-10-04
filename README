# NEUROX Main Node

The Main Node is the main controller for the actuator and temperature-control system of NEUROX.

It uses an ESP32 Dev Module to control the water therapy, pneumatic massage, and heating system.

## Functions

The Main Node controls and monitors:

* Warm water pump
* Cold water pump
* Air pump
* Solenoid valve
* Heating pad
* 2 × DS18B20 temperature sensors

## Hardware

| Component        | Purpose                                   |
| ---------------- | ----------------------------------------- |
| ESP32 Dev Module | Main controller                           |
| DS18B20 × 2      | Monitor water temperatures                |
| Warm water pump  | Circulate warm water                      |
| Cold water pump  | Circulate cold water                      |
| Air pump         | Generate air pressure                     |
| Solenoid valve   | Release air from the pneumatic system     |
| Heating pad      | Heat the warm water                       |
| Relay modules    | Control the pumps, valve, and heating pad |
| 12 V battery     | Power the actuators                       |

## Pin Configuration

| Component             | ESP32 GPIO |
| --------------------- | ---------: |
| Air pump relay        |     GPIO 4 |
| Warm water pump relay |    GPIO 19 |
| Cold water pump relay |    GPIO 21 |
| Solenoid valve relay  |    GPIO 18 |
| Heating pad relay     |    GPIO 22 |
| Warm water DS18B20    |    GPIO 17 |
| Cold water DS18B20    |    GPIO 16 |

The relay modules use active-LOW logic:

* `LOW` = ON
* `HIGH` = OFF

## Temperature Monitoring

Two DS18B20 temperature sensors are used to monitor the water temperatures.

* One sensor monitors the warm water.
* One sensor monitors the cold water.

The current target temperature for the warm water system is approximately **38°C**.

The heating pad is activated to heat the warm water and is switched off when the target temperature is reached.

## Water Therapy

The Main Node controls two water pumps:

* Warm water pump
* Cold water pump

The pumps are used to circulate water through the rehabilitation sleeve for temperature-based therapy.

## Pneumatic Massage

The Main Node controls an air pump and solenoid valve for the pneumatic massage system.

The air pump inflates the pneumatic system, while the solenoid valve is used to release the air.

The system uses timed inflation and release cycles for the massage function.

## Firebase

The Main Node can connect to Firebase to store and monitor system data.

Firebase credentials are stored locally in:

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
main_node/
├── main_node.ino
├── secrets.h
└── README.md
```

`se
