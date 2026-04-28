# HandRobotWithAI

This project represents the first stage of developing an AI-integrated robotic hand. This research was conducted as a final project for the fifth semester of my Bachelor’s degree in Informatics.

## 🚀 Development Phases (Code)

The source code in this repository documents the step-by-step evolution of the robot's capabilities, from basic movement to string processing.

### 📍 hand
The initial prototype phase designed to test basic mechanical features using three predefined servo positions to verify range of motion.

### 📍 handv2
Introduced direct control capabilities. Users can input raw PWM values (ranging from **150 to 600**) to manually adjust the position of each of the five fingers.

### 📍 handv4
A major logic update where each servo was specifically mapped into discrete levels. This simplified interaction, allowing users to input digits **1–6** to control the finger extension.

### 📍 handv5
Introduced **Alphabetical Support** for sign language gestures. While some signs are limited by the mechanical constraints of a single 5-finger hand, the system retains the 5-digit manual input feature for custom positioning.

### 📍 handv5-1
Integrated a **Bluetooth module**, enabling all previous control features to be executed wirelessly via a mobile device or remote serial terminal.

### 📍 handv5-2
Advanced the input logic by implementing a **Command Queue**. The robot can now process full strings and words, spelling them out automatically with timed intervals and an automatic "return-to-rest" sequence.

### 📍 handv6
The foundation for **AI Integration**. In this stage, the Arduino is configured to receive and process real-time serial data sent from a Python environment, enabling integration with computer vision models.

---

## 🛠 Hardware & Design
* **Mechanical Design:** The 3D model was acquired from open-source 3D platform communities and optimized for assembly.
* **Controller:** Arduino Nano / ESP32 (compatible).
* **Actuators:** PCA9685 PWM Driver with 5 Micro Servos.
* **Communication:** Serial (USB) and Bluetooth (HC-05/HC-06).

## 📂 Repository Structure
* `/code/handx`: Folders containing the Arduino `.ino` sketches for each development stage.
* `/design`: 3D printing files and assembly references.