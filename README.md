# NEUROX

NEUROX is a smart post-stroke rehabilitation system designed to support guided and monitored rehabilitation. It combines motion tracking, vibration feedback, pneumatic massage, and temperature-controlled warm and cold water therapy.

## System Structure

The NEUROX system consists of three main parts:

* **Web App** - Patient interface and physiotherapist dashboard for monitoring rehabilitation progress.
* **Main Node** - ESP32 Dev Module responsible for temperature monitoring, warm and cold water pumps, heating, pneumatic massage, and solenoid valve control.
* **Secondary Node** - ESP32-C3 responsible for MPU6050 motion tracking and vibration feedback.

## Repository Branches

| Branch           | Description                                            |
| ---------------- | ------------------------------------------------------ |
| `web`            | NEUROX web application                                 |
| `main-node`      | Main ESP32 firmware and actuator control               |
| `secondary-node` | ESP32-C3 firmware, IMU tracking and vibration feedback |

## Web App

The web application provides two main views:

* `/patient` - Patient rehabilitation interface, including session information, progress, badges, and recent sessions.
* `/dashboard` - Physiotherapist dashboard for monitoring patients, rehabilitation progress, completion rate, and range of motion.

The web app currently uses mock data while the Firebase backend and hardware data integration are being developed.

### Run the Web App

```bash
npm install
npm run dev
```

Open the local URL provided by Vite, usually:

```text
http://localhost:5173
```

## Main Node

The Main Node uses an ESP32 Dev Module to control the rehabilitation actuators and monitor water temperature.

Main functions include:

* Warm water temperature monitoring using DS18B20
* Cold water temperature monitoring using DS18B20
* Heating pad control
* Warm water pump control
* Cold water pump control
* Air pump control
* Solenoid valve control
* Pneumatic massage cycles
* Communication with the Secondary Node using ESP-NOW
* Firebase data updates

## Secondary Node

The Secondary Node uses an ESP32-C3 with two MPU6050 motion sensors and two vibration motors.

Main functions include:

* Monitoring movement using two MPU6050 IMUs
* Detecting when the user's movement becomes still
* Providing vibration feedback
* Sending commands to the Main Node using ESP-NOW
* Uploading movement and motor status to Firebase

## System Communication

The general system flow is:

```text
                 NEUROX WEB APP
                       │
                    Firebase
                       │
          ┌────────────┴────────────┐
          │                         │
     MAIN NODE                 SECONDARY NODE
     ESP32 Dev                  ESP32-C3
          │                         │
     ┌────┼────┬────┐          ┌────┴────┐
     │    │    │    │          │         │
   Water Heater Air  Valve     IMUs    Motors
   Pumps        Pump
          │
          └──── ESP-NOW ──────────────┘
```

## Technologies

* ESP32
* ESP32-C3
* MPU6050
* DS18B20
* Firebase
* React
* Vite
* Tailwind CSS
* ESP-NOW
* Recharts

## Development

The project is currently under development. The web application initially uses mock data while Firebase integration and hardware communication are being developed and tested.

Future development includes:

1. Connect the web app to Firebase.
2. Integrate real-time ESP32 sensor data.
3. Finalise the Firebase data structure.
4. Add authentication for patients and physiotherapists.
5. Improve rehabilitation monitoring and analytics.
6. Test the complete hardware and software system.
7. Deploy the web application.
8. Integrate the web app with Capacitor for mobile deployment.

## Project Goal

NEUROX aims to provide an affordable and integrated rehabilitation platform that combines physical therapy assistance, sensor-based movement monitoring, temperature therapy, pneumatic massage, and digital progress tracking.
