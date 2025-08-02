# IR-Sensor-Based-Vehicle-Speed-Detection
Detects vehicle speed using two IR sensors. Displays "NORMAL" or "OVERSPEED" on an I2C LCD and activates a buzzer if the speed exceeds a set threshold. Useful for traffic monitoring and speed control systems.

Project: IR Sensor-Based Vehicle Speed Detection

### 🔧 Platform
- **Microcontroller:** STM32F446RE (Nucleo-64)

### 📂 File
- [`main.c`](./ir-speed-detection/main.c)

### 📝 Description
Measures vehicle speed using two IR sensors and identifies overspeed violations. Displays results on an I2C LCD and triggers a buzzer when a violation is detected.

### 🔌 Pin Configuration
| Component        | Pin (STM32F446RE) | Description          |
|------------------|------------------|----------------------|
| IR Sensor 1      | PA8              | Detect vehicle entry |
| IR Sensor 2      | PB5              | Detect vehicle exit  |
| I2C LCD SDA      | PB9              | I2C Data             |
| I2C LCD SCL      | PB8              | I2C Clock            |
| Buzzer           | PC13             | Alert output         |
| LED (optional)   | PC0              | Overspeed indicator  |

### 💡 Logic
- Start timer when IR1 detects vehicle.
- Stop timer when IR2 detects.
- If time < 5s → Overspeed.
- Display result and sound buzzer if overspeed.

### 🛠️ Components
- STM32F446RE
- 2x IR Sensors
- I2C 16x2 LCD
- Buzzer
- Wires, Breadboard

---
