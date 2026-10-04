# ESP8266 Wi-Fi Controlled Robot Car

## 📌 Project Overview

This project is a **Wi-Fi Controlled Robot Car using ESP8266**. The robot can be controlled wirelessly from a smartphone through a Wi-Fi connection. It uses an **ESP8266 NodeMCU**, **L298N motor driver**, and **four BO DC motors** for movement.

The project also includes **speed control, LED/light control, buzzer/horn control, diagonal movement, and HC-SR04 ultrasonic reverse obstacle detection** for improved safety.

---

## 🚗 Features

* 📱 Smartphone-based Wi-Fi control
* 📡 ESP8266 Wi-Fi Access Point
* ⚙️ Four BO DC motors
* 🔌 L298N dual H-bridge motor driver
* 🎮 Forward movement
* ⬇️ Backward movement
* ⬅️ Left movement
* ➡️ Right movement
* ↖️ Forward-left diagonal movement
* ↗️ Forward-right diagonal movement
* ↙️ Backward-left diagonal movement
* ↘️ Backward-right diagonal movement
* 🚀 Speed control from **0–9 and maximum speed**
* 🔊 Buzzer / horn control
* 📏 HC-SR04 ultrasonic sensor
* 🛑 Automatic reverse obstacle detection
* ⚠️ Automatic stop when an obstacle is detected within **25 cm**
* 🔔 Automatic buzzer warning when an obstacle is detected
* 🌐 ESP8266 Access Point mode
* 💻 Web-based robot control

---

## 🛠️ Hardware Requirements

| Component                 |    Quantity |
| ------------------------- | ----------: |
| ESP8266 NodeMCU           |           1 |
| L298N Motor Driver        |           1 |
| BO DC Motors              |           4 |
| Robot Car Chassis         |           1 |
| HC-SR04 Ultrasonic Sensor |           1 |
| Buzzer                    |           1 |
| Battery / Power Supply    |           1 |
| Jumper Wires              | As required |
| Wheels                    |           4 |

---

## 🔌 Pin Connections

### L298N Motor Driver

| L298N Pin | ESP8266 Pin | Function              |
| --------- | ----------- | --------------------- |
| ENA       | D1          | Right motor speed     |
| IN1       | D2          | Right motor direction |
| IN2       | D3          | Right motor direction |
| IN3       | D4          | Left motor direction  |
| IN4       | D5          | Left motor direction  |
| ENB       | D6          | Left motor speed      |

### Buzzer

| Component     | ESP8266 Pin |
| ------------- | ----------- |
| Buzzer Signal | D7          |

### HC-SR04

| HC-SR04 Pin | ESP8266 Pin |
| ----------- | ----------- |
| VCC         | 5V          |
| TRIG        | D0          |
| ECHO        | D8          |
| GND         | GND         |

> ⚠️ **Important:** The HC-SR04 ECHO output can be approximately 5V, while ESP8266 GPIO works at 3.3V. Use a suitable voltage divider/level shifter between **ECHO and D8** to protect the ESP8266.

---

## 🔋 Power Supply

The L298N motor driver is powered from the motor battery.

The ESP8266 should be supplied with a suitable **5V regulated supply** through a USB/5V input or appropriate regulator.

### Important Power Notes

* Do not connect 12V directly to the ESP8266 5V/3.3V pin.
* Use a suitable voltage regulator/buck converter.
* Connect the **GND of the ESP8266, L298N, sensor, and power supply together**.
* Make sure the battery can provide sufficient current for four BO motors.
* Check the voltage and polarity before powering the circuit.

---

## 📡 Wi-Fi Control

The ESP8266 creates its own Wi-Fi network:

**Wi-Fi Name:** `Robot Car`
**Password:** `Samyak123`

After connecting the smartphone to the robot's Wi-Fi network, open:

```text
192.168.4.1
```

The robot control webpage can then be used to control the car.

---

## 🎮 Robot Controls

| Command | Function       |
| ------- | -------------- |
| F       | Forward        |
| B       | Backward       |
| L       | Left           |
| R       | Right          |
| G       | Forward Left   |
| I       | Forward Right  |
| H       | Backward Left  |
| J       | Backward Right |
| S       | Stop           |
| V       | Horn / Buzzer  |

The robot supports multiple speed levels from **0 to 9**, along with maximum speed.

---

## 📏 HC-SR04 Reverse Safety

The HC-SR04 sensor is used to detect obstacles behind the robot.

When the robot is moving backward:

1. HC-SR04 measures the distance behind the robot.
2. The ESP8266 compares the distance with the safety limit.
3. If an obstacle is detected within **25 cm**, the motors automatically stop.
4. The buzzer gives a warning beep.
5. The robot does not continue reversing while the obstacle remains within the safety distance.

The ultrasonic safety feature is primarily used during **backward and backward-diagonal movement**.

---

## ⚙️ Working Principle

The ESP8266 acts as the main controller of the robot.

```text
Smartphone
    ↓
Wi-Fi
    ↓
ESP8266 NodeMCU
    ↓
L298N Motor Driver
    ↓
4 BO Motors
    ↓
Robot Movement
```

For obstacle safety:

```text
HC-SR04
    ↓
Distance Measurement
    ↓
ESP8266
    ↓
Obstacle ≤ 25 cm?
    ↓
YES
    ↓
Stop Motors + Buzzer Warning
```

---

## 💻 Software Requirements

* Arduino IDE
* ESP8266 Board Package
* ESP8266WiFi Library
* ESP8266WebServer Library
* ArduinoOTA Library

### Required Libraries

```cpp
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>
```

---

## 📂 Project Structure

```text
ESP8266-WiFi-Robot-Car/
│
├── README.md
│
├── ESP8266_WiFi_Robot_Car.ino
│
├── circuit/
│   └── wiring.md
│
├── documentation/
│   └── project-documentation.pdf
│
└── images/
    └── robot-car.jpg
```

---

## 🚀 How to Upload the Code

1. Install **Arduino IDE**.
2. Install the ESP8266 board package.
3. Connect the ESP8266 NodeMCU to the computer using a USB data cable.
4. Select the correct ESP8266 board.
5. Select the correct COM port.
6. Open the `.ino` file.
7. Compile the program.
8. Upload the program to the ESP8266.
9. After successful upload, power the robot.
10. Connect your smartphone to the **Robot Car** Wi-Fi network.
11. Open `192.168.4.1` in a web browser.

> ⚠️ During programming/uploading, it is recommended to disconnect the motor driver and other external circuits if they interfere with ESP8266 boot mode.

---

## 🧪 Testing

The project can be tested in the following steps:

### Step 1 – Wi-Fi Test

Connect a smartphone to the **Robot Car** Wi-Fi network.

### Step 2 – Motor Test

Test:

* Forward
* Backward
* Left
* Right
* Diagonal movements
* Stop

### Step 3 – Speed Test

Test different speed levels from **0–9** and maximum speed.

### Step 4 – Buzzer Test

Activate the horn/buzzer from the control interface.

### Step 5 – Ultrasonic Test

Place an object behind the robot and test reverse movement.

When the object comes within approximately **25 cm**, the robot should stop and the buzzer should provide a warning.

---

## 🌟 Advantages

* Wireless robot control
* Smartphone-based operation
* Low-cost components
* Easy to build and modify
* Multiple movement directions
* Adjustable motor speed
* Reverse obstacle safety
* Automatic obstacle warning
* Suitable for IoT and embedded-system projects

---

## ⚠️ Limitations

* Wi-Fi range is limited when operating in Access Point mode.
* Robot performance depends on battery capacity.
* L298N has voltage loss and may generate heat.
* HC-SR04 performance can be affected by object shape and environmental conditions.
* The robot requires a stable power supply for reliable operation.

---

## 🔮 Future Scope

The project can be further improved by adding:

* Internet-based remote control
* Mobile application
* ESP32-based advanced version
* Camera/live video streaming
* GPS tracking
* Voice control
* IoT cloud monitoring
* Battery voltage monitoring
* Multiple ultrasonic sensors
* Autonomous navigation
* Obstacle avoidance
* Remote robot monitoring

---

## 🎓 Applications

This robot can be used for:

* College IoT projects
* Robotics demonstrations
* Embedded-system learning
* Wireless control experiments
* Engineering mini projects
* Smart vehicle prototypes
* Robotics competitions
* Educational purposes

---

## 📸 Project Images

Add your robot images inside the `images` folder.

Example:

```text
images/
├── robot-car.jpg
├── circuit-diagram.png
└── project-working.jpg
```

---

## 👨‍💻 Author

**Digital Tech / ESP8266 Wi-Fi Robot Car Project**

This project was developed as an educational **IoT and Robotics project** using ESP8266.

---

## 📄 License

This project is intended for **educational and academic purposes**.

You are free to study, modify, and improve the project for learning and non-commercial use.

---

## ⭐ Support

If you find this project useful, consider giving the repository a ⭐ **Star** on GitHub.
