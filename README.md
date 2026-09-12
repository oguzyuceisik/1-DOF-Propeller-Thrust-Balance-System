# 1-DOF-Propeller-Thrust-Balance-System
A 1-Degree-of-Freedom (1-DOF) closed-loop thrust balance arm controlled by a Brushless DC motor and an AS5600 12-bit magnetic rotary encoder, powered by an Arduino.
---

## Overview
This project demonstrates closed-loop attitude control of a thrust-balanced lever arm. A custom PID controller runs at a deterministic 50 Hz , stabilizing the arm against gravity at any desired angle. System parameters ($K_p, K_i, K_d$, and Target Angle) can be adjusted in real time via a 2-axis analog joystick and an interactive 1602 LCD menu without interrupting the control loop.

---

## Features
- **Deterministic Loop Timing:** Control task executes strictly at 50 Hz using non-blocking timer logic.
- **Custom Modular PID Implementation:** Lightweight and portable C library (`saptbsPID.c` / `saptbsPID.h`).
- **Integral Anti-Windup:** Prevents integrator saturation and aggressive overshoot using strict clamping boundaries ($\pm750\,\mu s$).
- **Base Actuator Offset:** Actuator output is mapped cleanly onto standard ESC pulse widths ($1000 - 2000\,\mu s$).
- **Live Telemetry & Interactive Menu:** Real-time angle monitoring and on-the-fly gain scheduling via 1602 LCD and analog joystick.

---

## Pinout Configuration

| Peripheral | Arduino Pin | Description |

| **ESC Signal** | D6 
| **AS5600 SDA** | A4
| **AS5600 SCL** | A5
| **Joystick X-Axis** | A0 
| **Joystick Y-Axis** | A1
| **Joystick Button** | D2 
| **LCD RS, EN** | D7, D8 
| **LCD D4 - D7** | D9, D10, D11, D12 

---

## Getting Started

1. Clone or download this repository.
2. Install the required Arduino libraries via Library Manager:
   - `Servo`
   - `Wire`
   - `LiquidCrystal`
   - `AS5600`
3. Connect the hardware according to the pinout table above.
4. Power the ESC with a 3S LiPo battery. The ESC BEC will provide regulated 5V to the Arduino and peripherals.
5. Upload the `.ino` sketch to your Arduino board.
6. Wait 2 seconds during the initialization delay for the ESC arming sequence.
7. Use the joystick to navigate the menu and tune $K_p, K_i, K_d$, or set your desired target angle.

---
