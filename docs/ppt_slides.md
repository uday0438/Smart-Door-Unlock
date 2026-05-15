# PPT Presentation: Smart Door Unlocking System

## Slide 1: Title Slide
- **Title:** Smart Door Unlocking System
- **Subtitle:** AI-Powered Security & Cloud Attendance
- **Tech Stack:** ESP32 | HuskyLens | Google Sheets
- **Presented By:** [Your Name]

---

## Slide 2: Introduction
- Traditional security systems are manual and slow.
- Modern problems require AI-driven solutions.
- Contactless and automated attendance is the need of the hour.

---

## Slide 3: Project Objectives
- Automated Door Control.
- AI-based Facial Recognition.
- Real-time Cloud Attendance Logging.
- Intruder Detection & Alerts.

---

## Slide 4: Key Features
- **Face Recognition:** High accuracy using HuskyLens.
- **IoT Enabled:** Instant sync with Google Sheets.
- **Intruder Alert:** Image capture and alarm system.
- **Live Status:** Real-time feedback on I2C LCD.

---

## Slide 5: Components Used
- ESP32 (Main Controller)
- HuskyLens (AI Camera)
- ESP32-CAM (Security Camera)
- SG90 Servo (Door Actuator)
- 16x2 LCD (Display)
- Buzzer & LEDs

---

## Slide 6: System Architecture
- **Input:** Face Data (HuskyLens).
- **Processing:** Logic Verification (ESP32).
- **Output:** Door Unlock (Servo) / Alarm (Buzzer).
- **Cloud:** Attendance Storage (Google Sheets).

---

## Slide 7: Working Principle (Authorized)
- Face identified by HuskyLens.
- Match found in database.
- Green LED ON & Door Unlocks.
- "Present" marked in Google Sheets.

---

## Slide 8: Working Principle (Unauthorized)
- Unknown face detected.
- Red LED ON & Buzzer Alarm.
- ESP32-CAM captures intruder image.
- Intruder log created in cloud.

---

## Slide 9: Advantages
- High Security & Accuracy.
- Completely Contactless.
- Automated Record Management.
- Low Cost & Scalable.

---

## Slide 10: Conclusion & Future Scope
- **Conclusion:** Successfully integrated AI and IoT for security.
- **Future:** Mobile app integration, Telegram alerts, and live video streaming.
- **Thank You! Questions?**
