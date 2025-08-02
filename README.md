🚗 IR-Based Speed Analyzer with STM32  
**Short Description:** Detects and displays vehicle speed violations using two IR sensors.

### Description:
This project is built with STM32 and uses two IR sensors to detect vehicle speed based on time difference between sensor triggers. If a vehicle crosses the threshold speed (based on a time difference < 5s), an alert is displayed, and a buzzer is triggered.

### Components:
- STM32F103 (Blue Pill)
- 2x IR Sensors
- I2C 16x2 LCD
- Buzzer
- LED

### Pin Configuration:
| Component | STM32 Pin |
|----------|------------|
| IR Sensor 1 | PA8 |
| IR Sensor 2 | PB5 |
| Buzzer | PC1 |
| Speed Alert LED | PA5 |
| LCD I2C | I2C1 (PB6 - SDA, PB7 - SCL) |

### Features:
- Counts vehicles and overspeed incidents
- Displays speed status and counters on LCD
- Buzzer alert on overspeed
- Real-time LCD scroll for system title

---
