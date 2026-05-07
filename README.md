# ESP32 Proportional Differential Drive Controller

This repository contains the Arduino/C++ code for an ESP32-based RC car utilizing a differential drive system. Unlike standard "on/off" remote controls, this project interprets analog joystick/lever coordinates (X and Y) to dynamically adjust both the acceleration and the turning angle for smooth, proportional control.

## Key Features
* **Standalone WiFi AP:** The ESP32 hosts its own Soft Access Point (Hotspot) named `ESP32_RC_Car`, allowing direct smartphone or controller connections without needing an external router.
* **TCP Socket Communication:** The microcontroller listens for incoming client commands via a WiFiServer running on Port 80.
* **Proportional Speed Control:** Uses 10-bit PWM resolution (0-1023) to provide fine-grained motor control for smooth acceleration and deceleration.
* **Dynamic Steering Logic:** The code parses X and Y axis values to calculate a steering ratio (`motorSpeedX = ratio * motorSpeed`). This creates a true differential drive by dynamically slowing down the inner wheel during a turn while maintaining the vehicle's overall momentum.
* **Joystick Deadzones:** Implements resting deadzones around the joystick's center point (ignoring values between 505 and 515) to prevent accidental motor humming or drifting when the lever is idle.
* **Safety Cutoff:** Continuously monitors the connection status and automatically stops all motors if the WiFi client drops or disconnects.

## Hardware Pinout
The code is configured to interface with a standard dual-channel DC motor driver (such as an L298N or TB6612FNG) using the following ESP32 GPIO pins:

* **Left Motor (A):**
  * Enable (PWM): GPIO 4
  * IN1: GPIO 27
  * IN2: GPIO 14
* **Right Motor (B):**
  * Enable (PWM): GPIO 18
  * IN3: GPIO 12
  * IN4: GPIO 19

## Serial & Debugging
* **Baud Rate:** `115200`
* **Spy Mode:** The ESP32 prints the exact characters sent by the app to the Serial Monitor (e.g., `App Sent: Y`), making it easy to debug the connection and app formatting.

## Communication Protocol
The ESP32 expects commands sent over the WiFi socket in the following formats:

* **`Y<value>`:** Controls the forward/backward axis. A value of `511` is center. Values > `515` drive the car forward, and values < `505` drive it backward in proportion to the distance from the center.
* **`X<value>`:** Controls the steering axis. It calculates the speed reduction needed for the left or right wheel to execute the turn based on the current `Y` speed.
* **`S` or `s`:** Triggers `stopCar()`, pulling all driver pins LOW and setting PWM to 0.
* **`L`, `l` / `R`, `r`:** Executes a hard tank-turn (motors spin in opposite directions) at maximum PWM (`1023`).