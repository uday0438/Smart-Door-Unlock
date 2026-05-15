# Smart Door Unlocking System 🚀

AI-powered Smart Door Unlocking System using **ESP32**, **HuskyLens**, and **ESP32-CAM** with facial recognition, cloud attendance logging, and intruder detection.

## 📌 Project Overview
This project is an AI-based security solution that automates door unlocking and attendance tracking. It uses the HuskyLens AI camera to recognize faces and an ESP32 to control the hardware and sync data with Google Sheets.

## ✨ Features
- **AI Face Recognition:** Recognizes trained faces instantly.
- **Smart Door Lock:** Controls a servo motor to unlock the door.
- **Cloud Attendance:** Logs entry/exit data directly to Google Sheets.
- **Intruder Alert:** Captures images of unknown persons and triggers an alarm.
- **Live Display:** Shows time and status on a 16x2 I2C LCD.

## 🛠️ Components
- ESP32 Dev Board
- HuskyLens AI Camera
- ESP32-CAM Module
- SG90/MG996R Servo Motor
- 16x2 I2C LCD
- Buzzer & LEDs
- 5V Power Supply

## 🔌 Pin Connections
| Component | ESP32 Pin |
| :--- | :--- |
| HuskyLens TX | GPIO16 |
| HuskyLens RX | GPIO17 |
| Servo Signal | GPIO14 |
| Buzzer | GPIO15 |
| Green LED | GPIO4 |
| Red LED | GPIO2 |
| LCD SDA | GPIO21 |
| LCD SCL | GPIO22 |
| ESP32-CAM TX | GPIO26 |
| ESP32-CAM RX | GPIO25 |

## 🚀 Setup Instructions
1. **Arduino IDE:** Install ESP32 boards and required libraries (`HUSKYLENS`, `ESP32Servo`, `LiquidCrystal_I2C`).
2. **Google Sheets:** Create a new sheet named "Attendance".
3. **Apps Script:** Copy the code from `Google_AppScript/` and deploy it as a Web App.
4. **Hardware:** Flash the `ESP32_Main_Controller` and `ESP32_CAM` codes.
5. **Training:** Train authorized faces on the HuskyLens (IDs 1-7).

## 📂 Directory Structure
- `ESP32_Main_Controller/`: Primary logic and WiFi sync.
- `ESP32_CAM/`: Security camera capture logic.
- `Google_AppScript/`: Backend logic for data storage.
- `docs/`: Report and PPT presentation slides.

## 🤝 Contributing
Feel free to fork this project and submit pull requests!

## 📜 License
This project is licensed under the MIT License.
