VENAT Smart

An Arduino-based smart system with IoT integration and AI-powered crop analysis
VENAT Smart combines sensor-driven automation with predictive insights to help farmers monitor, control, and optimize plant growth.


## Features

### 🔹 IoT Data Collection

* Temperature, humidity, light intensity, and soil moisture sensors
* Real-time data transmission via ESP32 to **Thinger.io dashboard**
* Remote environmental control (ventilation, heating, cooling, irrigation, lighting)

👉 [Click here to view the VENAT Dashboard](https://console.thinger.io/dashboards/VENAT_SMART_GREENHOUSE?authorization=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJqdGkiOiJEYXNoYm9hcmRfVkVOQVRfU01BUlRfR1JFRU5IT1VTRSIsInN2ciI6ImV1LWNlbnRyYWwuYXdzLnRoaW5nZXIuaW8iLCJ1c3IiOiJBbmRyZXdQaGlyaSJ9.LoiY1xUMc0c5njl6YqUD403JNm0sD2vwEiFh5kQLI0s)

### 🔹 AI-Driven Analysis

* Pest and Disease Prediction: Detects abnormal growth patterns and environmental risks
* Soil Health Monitoring: Tracks nutrient levels & moisture balance
* Event Discovery: Identifies unusual activity (e.g., sudden drops in humidity, pest risks)
* Growth Forecasting (proprietary model – removed from public version)

### 🔹 Energy & Cost Tracking

* Monitors power usage of greenhouse systems
* Provides cost estimation for sustainable farming


## Libraries & Dependencies

* Arduino Core for ESP32
* DHTxx Sensor Library (Temperature & Humidity)
* BH1750 / LDR Library (Light intensity)
* Soil Moisture Library (Analog)
* hinger.io Arduino Client (IoT Dashboard integration)
* Custom AI Model API (Proprietary – excluded from public repo)

## Repository Contents

* `src/` → Arduino sketches for sensor reading & control logic
* `docs/` → Technical documentation & diagrams
* `dashboard/` → Thinger.io dashboard integration guide
* `README.md` → Project overview (this file)

## What’s Public vs. Proprietary

✅ Open / Educational:

* Sensor data collection
* Basic control logic
* Dashboard integration

❌ Proprietary (removed in this repo):

* Plant growth prediction models
* Pest & disease AI algorithms
* Soil nutrient management logic

## Demo Video

🎥 https://drive.google.com/file/d/11I0zw3EE4czZghmemPag5roFIdtp2fmd/view?usp=drivesdk
