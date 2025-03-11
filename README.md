Cloud Integration Cargo Management System
Brief Overview
The Cloud Integration Cargo Management System is designed to monitor cargo conditions in real time using IoT sensors and ESP32 microcontroller. It ensures cargo safety by tracking environmental factors such as temperature, humidity, and gas levels. Data collected is stored in Firebase, providing remote access and monitoring.

Tools & Components Used
ESP32

A Wi-Fi-enabled microcontroller used to collect sensor data and send it to Firebase.
Key Role: Acts as the brain of the system, connecting all sensors and sending real-time updates to the cloud.
MQ-7 Sensor

A carbon monoxide (CO) gas sensor that detects harmful gas levels in cargo.
Key Role: Ensures safety by detecting gas leaks and preventing hazardous situations.
HTS11 Sensor

A temperature and humidity sensor used to monitor cargo storage conditions.
Key Role: Helps maintain optimal conditions for temperature-sensitive cargo like food or pharmaceuticals.
Firebase

A cloud database used to store sensor readings.
Key Role: Provides real-time data access and remote monitoring via a mobile or web application.
Connections & Data Flow
Sensors (MQ-7, HTS11) collect real-time data.
ESP32 reads sensor values and processes them.
ESP32 sends the data to Firebase via Wi-Fi.
A web or mobile application retrieves and displays the data for monitoring.
Alerts are triggered if temperature, humidity, or gas levels exceed safe limits.

Connections of Sensors with ESP32
1. ESP32 Microcontroller (Central Unit)
The ESP32 acts as the main processing and communication unit.
It collects data from sensors and transmits it to Firebase over Wi-Fi.
It operates at 3.3V logic level, making it compatible with most sensors.
2. MQ-7 Sensor (Gas Sensor) Connection
Purpose: Detects Carbon Monoxide (CO) levels.

Connections:
VCC → 5V (ESP32) (Some variants work with 3.3V)
GND → GND (ESP32)
A0 (Analog Output) → Analog Pin (ESP32, e.g., GPIO34)
Working:
The sensor detects CO gas and produces an analog voltage output.
ESP32 reads this voltage level and converts it to a gas concentration value.
If CO levels exceed a threshold, an alert is triggered.
4. HTS11 Sensor (Temperature & Humidity) Connection
Purpose: Measures temperature and humidity inside the cargo.

Connections:
VCC → 3.3V (ESP32)
GND → GND (ESP32)
DATA → Digital Pin (ESP32, e.g., GPIO21)

Working:
The sensor sends digital data over a single-wire protocol.
ESP32 reads the data and converts it into temperature (°C) and humidity (%).
If values exceed safe limits, an alert is sent to Firebase.
5. Firebase Cloud Connection
ESP32 sends sensor data to Firebase using Wi-Fi.

Steps:
ESP32 reads sensor values.
It processes and formats the data.
It sends data to Firebase using HTTP or Firebase SDK.
A web/mobile app retrieves the data and displays it to the user.
Alerts are triggered if abnormal values are detected.

Final Workflow Summary
✅ Sensors collect data → ESP32 processes it → Data is sent to Firebase → User accesses it remotely → Alerts for critical conditions
Key Features
✅ Real-time Monitoring: Live updates on cargo conditions.
✅ Cloud Storage: Data is securely stored in Firebase.
✅ Safety Alerts: Notifications for abnormal gas, temperature, or humidity levels.
✅ Remote Access: View data from anywhere using a web or mobile app.
