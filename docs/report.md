# Project Report: Smart Door Unlocking System

## 1. Introduction
The Smart Door Unlocking System is a cutting-edge IoT solution designed to automate access control and attendance monitoring using Artificial Intelligence. In traditional security systems, manual verification or card-based access is often slow and prone to "buddy punching" or lost credentials. This project leverages facial recognition technology (HuskyLens) and IoT connectivity (ESP32) to provide a seamless, secure, and automated environment.

### 1.1 Problem Statement
Educational institutions and corporate offices face challenges in managing attendance and security. Traditional methods are:
- Time-consuming.
- Prone to errors.
- Hard to monitor in real-time.
- Susceptible to unauthorized entry.

### 1.2 Proposed Solution
Our system provides a dual-benefit:
1. **Automated Attendance:** Recognized students/employees are marked "Present" in a Google Sheet automatically.
2. **Enhanced Security:** Intruder detection triggers a camera capture and an alarm, logging the incident instantly.

---

## 2. Project Objectives
- To design a facial recognition-based door locking mechanism.
- To implement real-time cloud-based attendance logging using Google Sheets.
- To develop an intruder detection system with image capture capabilities.
- To provide a user-friendly status display via an I2C LCD.
- To ensure a contactless and hygienic access control method.

---

## 3. System Architecture
The system follows a modular architecture consisting of:
- **Perception Layer:** HuskyLens for face scanning and ESP32-CAM for intruder snapshots.
- **Control Layer:** ESP32 Dev Board managing logic, timing, and peripheral control.
- **Actuation Layer:** Servo motor for physical locking/unlocking and Buzzer/LEDs for alerts.
- **Network Layer:** WiFi connectivity to communicate with Google Cloud.
- **Application Layer:** Google Sheets and Apps Script for data storage and analysis.

### 3.1 Data Flow
1. User approaches the door.
2. HuskyLens identifies the face ID.
3. ESP32 verifies the ID against the internal database.
4. If authorized:
   - Door unlocks (Servo rotates).
   - LCD displays a welcome message.
   - Attendance is sent to Google Sheets.
5. If unauthorized:
   - Alarm triggers.
   - ESP32-CAM captures the image.
   - Intruder log is created in the cloud.

---

## 4. Hardware Requirements
| Component | Specification | Purpose |
| :--- | :--- | :--- |
| **ESP32 WROOM-32** | 30-pin Dev Board | Central Processing & WiFi |
| **HuskyLens** | AI Vision Sensor | Face Detection & ID Recognition |
| **ESP32-CAM** | OV2640 Lens | Intruder Image Capture |
| **SG90 Servo** | 180° Micro Servo | Physical Door Lock Actuator |
| **16x2 LCD** | I2C Interface | Status & Clock Display |
| **Buzzer** | Active 5V | Audible Alert for Intruders |
| **LEDs** | Red & Green | Visual Indicators |

---

## 5. Software Requirements
- **Arduino IDE:** For coding and flashing the ESP32 boards.
- **Google Apps Script:** To bridge the ESP32 and Google Sheets.
- **Google Sheets:** As the primary database.
- **Libraries:**
  - `HUSKYLENS.h`
  - `WiFi.h`
  - `ESP32Servo.h`
  - `LiquidCrystal_I2C.h`

---

## 6. Circuit Diagram & Pin Mapping
### 6.1 ESP32 (Main) Connections
- **HuskyLens:** TX -> GPIO16, RX -> GPIO17
- **Servo:** Signal -> GPIO14
- **Buzzer:** Signal -> GPIO15
- **Green LED:** GPIO4
- **Red LED:** GPIO2
- **LCD:** SDA -> GPIO21, SCL -> GPIO22
- **ESP32-CAM (UART):** TX -> GPIO26, RX -> GPIO25

---

## 7. Implementation Details
### 7.1 Facial Recognition Training
Before deployment, authorized faces are "learned" by the HuskyLens. Each face is assigned a unique ID (1-7 in this project).

### 7.2 Attendance Periods
The system operates on pre-defined periods. For example, a "Period 1" window allows for IN and OUT attendance tracking within a 30-second window.

### 7.3 Google Sheets Integration
Using the `HTTPClient` library, the ESP32 sends a GET request to the Google Apps Script Web App URL. The script then appends a row with:
- Timestamp
- Name
- Hall Ticket
- ID
- Period
- Status (IN/OUT/Final)

---

## 8. Testing and Results
The prototype was tested under various lighting conditions.
- **Accuracy:** >95% for recognized faces.
- **Latency:** Cloud sync takes approx. 1-2 seconds.
- **Security:** Intruder capture was successfully triggered upon detecting an unknown face.

---

## 9. Future Scope
- **Mobile App:** Integrating with Blynk or a custom React Native app for remote monitoring.
- **Telegram Bot:** Real-time alerts with intruder photos sent directly to the owner's phone.
- **Face Mask Detection:** Updating the AI model to recognize faces even with masks.
- **Multi-Factor Auth:** Adding an RFID or Keypad backup.

---

## 10. Conclusion
The Smart Door Unlocking System is a robust, cost-effective, and scalable solution for modern security needs. By combining AI at the edge with cloud logging, it offers a level of automation that significantly reduces human intervention and improves reliability.
