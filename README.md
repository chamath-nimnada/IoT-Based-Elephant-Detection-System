# IoT-Based Elephant Detection System

## 🐘 Problem Identification

In Sri Lanka, elephant-train collisions are a recurring issue due to elephants crossing railway lines that intersect their natural habitats. These incidents endanger wildlife, damage infrastructure, and jeopardize human lives. A real-time, energy-efficient, location-aware detection and alert system is needed to mitigate this risk.

---

## 🌐 Project Overview

This project is a solar-powered, IoT-based system designed to detect elephant presence near and on railway tracks. It uses motion and distance sensors, tilt monitoring, and cloud-based alerting to warn train operators and control rooms in real time.

Key features include:

- Dual-sensor detection (PIR + Ultrasonic) for accurate elephant classification
- Tower tilt monitoring via tilt sensor
- Real-time data sync with Firebase via Wi-Fi
- Unique device ID for map-based identification
- Solar-powered system with battery backup
- Weatherproof outdoor deployment

---

## ⚙️ Functional Description

- 🐘 Elephant Classification

  - PIR only: Elephant nearby (not on track)
  - PIR + Ultrasonic: Elephant on track
  - Ultrasonic only: Ignored (false positive)

- 📡 Real-Time Alerts  
  ESP32 pushes data to Firebase when elephant presence is detected.

- 🧭 Device Location  
  Each device is assigned a unique ID tied to a predefined map location.

- ↻ Data Logging  
  Firebase stores all events with timestamp, battery level, and status.

- 🧭 Tower Tilt Detection  
  A tilt sensor flags possible physical tampering or toppling.

- ☀️ Autonomous Power  
  Solar panel charges Li-ion battery via TP4056 module.

---

## 🔧 System Components

### 🛠️ Hardware

- ESP32 microcontroller (Wi-Fi enabled)
- PIR sensor (HC-SR501)
- Ultrasonic sensor (HC-SR04)
- Tilt switch sensor
- 3.7V 2000mAh Li-ion battery
- TP4056 charging module
- 20W solar panel
- Voltage divider circuit (battery monitoring)
- IP65 waterproof enclosure

### 💻 Software

- Arduino C++ firmware
- Firebase Realtime Database
- Web-based dashboard for real-time map + alerts

---

## 📡 Communication Flow

1. PIR & Ultrasonic sensors detect elephant presence
2. ESP32 processes input and determines detection status
3. ESP32 sends status (e.g. on_track, near_track, clear) to Firebase with device ID and battery level
4. Firebase updates Web Dashboard in real time
5. Train control center receives live visual + audio alerts

---

## 🔋 Power System

- 20W Solar Panel (daytime charging)
- 3.7V 2000mAh Li-ion battery (energy storage)
- TP4056 module with overcharge/overdischarge protection
- System designed for 24/7 operation in remote locations

---

## 🛆 Deployment

- Devices mounted on towers along railway tracks
- Each unit has unique ID and location on dashboard
- Units installed at 20–25 meter intervals for effective coverage
- Enclosures are fully weather-sealed (IP65 rated)
