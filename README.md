# IoT-Based Elephant Detection System

## 🐘 Problem Identification

In Sri Lanka, trains are a key mode of public transportation, often passing through towns and forested areas. A serious issue arises when elephants crossing railway tracks are struck by oncoming trains. These collisions occur frequently due to:

- Elephants' natural movement patterns
- Trains' inability to stop quickly

This situation endangers the elephant population, disrupts railway operations, and poses serious safety risks for passengers. A real-time detection and alert system is needed to prevent such incidents.

---

## 🌐 Project Overview

The goal is to develop an **IoT-based system** that detects elephants near railway tracks and sends real-time alerts to train operators. The system includes:

- **PIR sensors** and **accelerometers**
- **Solar-powered** operation
- **Location-based** alert messages via **LoRaWAN**

---

## ⚙️ Functional Description

- **Elephant Detection**  
  Uses PIR sensors to detect movement within 20 meters by sensing body heat (infrared changes).

- **Tower Integrity Monitoring**  
  An accelerometer detects if the tower is damaged or tilted due to elephant activity.

- **Location Tracking**  
  GPS module provides the exact location of the tower for accurate alerts.

- **Real-Time Alerts**  
  Alerts are sent via LoRaWAN to train drivers and control centers.

- **Solar Power Supply**  
  Solar panels charge onboard batteries, enabling autonomous operation in remote areas.

- **Data Processing**  
  An ESP32 microcontroller filters false positives and manages data transmission to a cloud platform.

---

## 🔧 System Components

### 🛠️ Hardware

- **PIR Sensor**: e.g., HC-SR501
- **Accelerometer**: e.g., ADXL345
- **Microcontroller**: e.g., ESP32
- **GPS Module**: e.g., NEO-6M
- **Power System**: 10–20W Solar Panel + 3.7V, 2000mAh Li-ion Battery
- **Enclosure**: Weatherproof box for outdoor deployment

### 💻 Software

- **Firmware** for ESP32 (sensor data processing + communication)
- **Cloud Platform**: e.g., AWS IoT Core (data storage + alert management)
- **Notification System**: Mobile App or SMS Gateway for real-time alerts

---

## 📡 Communication Flow

1. Sensors detect elephant movement or tower tampering.
2. Data is processed by ESP32 microcontroller.
3. GPS location is attached.
4. Alert is sent via LoRaWAN.
5. Cloud platform distributes the alert to appropriate recipients (e.g., control room, train driver).

---

## 🔋 Power System

- Solar panel (10–20W)
- Li-ion rechargeable battery (3.7V, 2000mAh)
- Ensures 24/7 operation in remote environments

---

## 📦 Deployment

- Mounted on towers along railway tracks
- Weatherproof enclosures protect electronic components
- Units spaced based on detection range (~20m coverage per unit)
