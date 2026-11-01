ESP32 based intrusion Detection system with UART debugging and telegram alert

1. Project Overview

This project is an ESP32-based Smart Intrusion Detection and Security System designed to detect unauthorized entry using an IR obstacle sensor.

When an intrusion is detected, the ESP32 processes the sensor signal and activates the security alert system. It provides LED status indication, relay-controlled buzzer alerts, a Wi-Fi web dashboard, and real-time Telegram notifications.

2. Project Objectives
Detect unauthorized movement using an IR sensor.
Monitor security status through a web dashboard.
Generate audio and visual alerts.
Send real-time Telegram notifications.
Implement UART serial debugging.
3. Hardware Components
ESP32 Development Board
FC-51 IR Obstacle Sensor
1-Channel Relay Module
Buzzer
Red LED
Yellow LED
Green LED
220Ω Resistors
Breadboard
Jumper Wires
4. Software and Tools
Visual Studio Code
PlatformIO IDE
Embedded C/C++
Arduino Framework
Wi-Fi WebServer
Telegram Bot API
UART Serial Communication
GitHub
5. Pin Configuration
Component	ESP32 GPIO
IR Sensor OUT	GPIO 27
Red LED	GPIO 25
Yellow LED	GPIO 32
Green LED	GPIO 33
Relay IN	GPIO 26
6. Key Features
IR-based intrusion detection
ARM and DISARM security modes
Three LED status indicators
Relay-controlled buzzer alert
Wi-Fi connectivity
Web-based security dashboard
Telegram real-time alert notifications
UART serial debugging
7. Working Principle
The IR sensor monitors the detection area.
When an object is detected, the sensor sends a signal to the ESP32.
The ESP32 processes the input signal.
The system activates the corresponding LED and buzzer alert.
Telegram sends an intrusion notification.
The web dashboard displays the security system status.
8. Project Demonstration

YouTube Project Demo: https://youtube.com/shorts/gsx4jYNTWeM?si=vO7lqzusC2Fl-6rO

Watch Project Demonstration

9. Applications
Home Security
Office Security
Industrial Monitoring
Restricted Area Protection
10. Future Enhancements
ESP32-CAM integration
Mobile application monitoring
Cloud-based event logging
Advanced security features
11. Author

Bala Krishnan M.

BE – Electrical and Electronics Engineering

Project Domain: Embedded Systems | IoT | Firmware Development
