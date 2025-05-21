# 💧 WaterSafe Distance Meter

A real-time water level monitoring system using an ARM Cortex-M3 microcontroller and HC-SR04 ultrasonic sensors. Designed to prevent overflows and promote sustainable water management through intelligent alerts and visual feedback.

---

## 🚀 Features

- 📏 **Distance Monitoring** with HC-SR04 ultrasonic sensor
- 💡 **LED Alerts** for water levels below 2.5 feet
- 🖥️ **LCD Display** for real-time distance feedback
- 🔊 **Buzzer Warning System** for critical proximity
- 🌍 **Aligned with SDG 6** – Clean Water and Sanitation

---

## 🧠 Working Principle

The HC-SR04 sensor emits ultrasonic waves and measures the echo time to calculate the distance to the water surface. If the water level is too high (i.e., distance < 2.5 ft), a buzzer and LED are triggered.

---

## 🔧 Tech Stack

| Component        | Description                                           |
|------------------|-------------------------------------------------------|
| MCU              | ARM Cortex-M3 LPC1768 board                          |
| Sensor           | HC-SR04 Ultrasonic Distance Sensor                   |
| Programming      | Embedded C using Keil MicroVision                    |
| Interface        | LCD Display, LEDs, Buzzer                            |
| Flashing Tool    | Flash Magic                                           |

---

## 🛠️ Hardware Setup

- ARM Cortex-M3 Development Board (ALS-SDA-ARMCTXM3-01)
- HC-SR04 Ultrasonic Sensor
- LCD Display Module
- Buzzer + LED Array
- USB-to-B Cable + Power Supply

🖼️ **Hardware Connections**  
![ Ultrasonic HC-SR04 module timing diagram](https://github.com/user-attachments/assets/8112edb2-7b11-4e3b-ade6-480abcdf5cb5)


🖼️ **Circuit Diagram**  
![Circuit Diagram]([assets/circuit_diagram.jpg](https://github.com/user-attachments/assets/b9bc27fb-ee90-4756-b70b-6a58a0cf64ac))

---

## ⚙️ System Behavior

| Distance Range     | Output Behavior                                   |
|--------------------|----------------------------------------------------|
| > 2.5 ft           | LCD displays distance only                         |
| < 2.5 ft           | LCD + Buzzer + LED Alert                          |
| Variable thresholds| LED intensity varies as distance decreases        |

🖼️ **Sample Output - Safe Distance**  
![Output Safe](https://github.com/user-attachments/assets/7d6379ec-dc84-48c2-95a6-b93b9d5bdd7a)


🖼️ **Sample Output - Alert Triggered**  
![Output Alert](https://github.com/user-attachments/assets/4798839d-5d5e-4ee5-8973-86c4d2c36e69)

---

## 📚 References

- [What is an Ultrasonic Sensor](https://robocraze.com/blogs/post/what-is-ultrasonic-sensor)
- [WaterSafe Proximity Sensors](https://watersafetech.com/benefits-of-using-watersafe-proximity-sensors)


