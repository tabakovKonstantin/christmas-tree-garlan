# 🎄 Christmas Tree Garland

A smart and customizable Christmas tree garland designed for integration with **Home Assistant**. The garland creates a vibrant holiday atmosphere, allowing full control over effects, colors, and brightness via MQTT.

## 🚀 Features
- 🌟 **Wi-Fi Setup via Access Point**: Automatically creates a Wi-Fi access point on first power-up for easy configuration.
- 🔌 **MQTT Integration**: Seamlessly integrates with Home Assistant or other MQTT-based platforms.
- 🏡 **Home Assistant Discovery**: Automatically detected as a light entity in Home Assistant.
- 🎨 **Custom Effects**: Choose from a variety of lighting effects, colors, and brightness levels.


## 📦 How It Works

1. **Initial Setup**:
   - When powered on for the first time, the garland creates a Wi-Fi access point.
   - Connect to the access point (`Garland-123456` or similar).
   - Open the configuration page (e.g., `http://192.168.4.1`) in your browser.
   - Enter your Wi-Fi credentials and MQTT broker details (server, port, username, password).

2. **MQTT Integration**:
   - After successful Wi-Fi connection, the garland connects to the specified MQTT broker.
   - It announces itself to Home Assistant via **MQTT Discovery** as a light entity.

3. **Control via Home Assistant**:
   - The garland appears as a light in Home Assistant with controls for:
     - On/Off
     - Brightness
     - Color
     - Effects (e.g., blinking, fading, color cycling).

4. **Customization**:
   - Use Home Assistant to create automations or integrate with scenes for a fully synchronized holiday experience.

### Requirements

- **Hardware**:
  - ESP8266 ESP-12E or ESP32 microcontroller. [AliExpress](https://www.aliexpress.com/item/4000550036826.html?spm=a2g0o.order_list.order_list_main.65.2abd1802cjdPx5)
  - Addressable RGB LEDs (e.g., WS2812B, SK6812). [AliExpress](https://www.aliexpress.com/item/1005002247063442.html?spm=a2g0o.order_list.order_list_main.70.2abd1802cjdPx5)
  - Power Supply. [AliExpress](https://www.aliexpress.com/item/1005003112787694.html?spm=a2g0o.order_list.order_list_main.64.2abd1802cjdPx5)
- **Software**:
  - MQTT broker (e.g., Mosquitto).
  - Home Assistant (optional, but recommended for full functionality).

## 🛠️ How to Build

1. **Prepare the components:**
   - **ESP8266** (e.g., NodeMCU or Wemos D1 mini)
   - **LED Strip** (e.g., WS2812)
   - **Power Supply** (5V or as required by your LED strip)
   - **330 Ohm Resistor** for the data line protection
   - **1000 µF Capacitor** for power stabilization (recommended for long LED strips)

2. **Connections:**
   - Connect the **VIN** and **GND** of the **Power Supply** to the **VIN** and **GND** pins of the **ESP8266**.
   - The **Data** line from the **LED Strip** is connected to **D3 (GPIO0)** on the **ESP8266** through a **330 Ohm Resistor**.
   - Ensure the **Power Supply** provides enough current to power both the **ESP8266** and the **LED Strip**.

### Wiring Diagram

Below is the wiring diagram for connecting the **ESP8266** to the **LED Strip** and power supply:
```
+-----------------+       +-----------------+               +-----------------+
|    Power Supply |       |   ESP8266       |               |   LED Strip     |
|                 |       |                 |               |                 |
|   +---------+   |       |                 |               |                 |
|   |         |   |       |                 |               |                 |
|   |  VIN    |---|-------|--VIN------------|---------------|--VIN            |
|   |  GND    |---|-------|--GND------------|---------------|--GND            |
|   +---------+   |       |                 |               |                 |
+-----------------+       |                 |  +---------+  |                 |
                          |      D3 (GPIO0) |--| Resistor|--|--Data           |
                          |                 |  | 330 Ohm |  |                 |
                          +-----------------+  +---------+  +-----------------+
```
2. **STL Model for 3D Printing**:
   - A custom STL model is provided in the repository for printing a case to house the ESP8266 and protect the wiring.
   - The STL file is located in the `/assets` directory of this repository.
   - To print:
     - Use your favorite slicer software (e.g., Cura, PrusaSlicer).
     - Recommended settings: **PLA filament**, **0.2mm layer height**.

## 🔧 Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/tabakovKonstantin/christmas-tree-garland.git
   cd christmas-tree-garland

2. Flash the firmware to your microcontroller:
   - Open the project in [PlatformIO](https://platformio.org/) (e.g., via VS Code).
   - Connect your microcontroller to the computer via USB.
   - Simply press the **Upload** button in PlatformIO. The firmware will be compiled and uploaded automatically.

3. Power on the garland and complete the initial setup as described above.

## 📄 License
  
  This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
  
  ---
  
  Enjoy your festive lighting and have a wonderful holiday season! 🎅✨

