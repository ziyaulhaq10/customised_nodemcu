# 🛠️ Customised NodeMCU Web Interface

This project provides a configurable web interface for NodeMCU (ESP8266) GPIO pins. Users can control and label pins through a JSON configuration stored in EEPROM. The interface dynamically updates based on the configuration, offering a flexible solution for various applications.

---

## 🚀 Features

* **Dynamic Web Interface**: Automatically generates control buttons for configured GPIO pins.
* **JSON Configuration**: Customize pin labels and visibility through a JSON file.
* **EEPROM Storage**: Persist configurations across reboots.
* **mDNS Support**: Access the device using a hostname (e.g., `http://office.local`).
* **Responsive Design**: User-friendly interface accessible from various devices.

---

## 📁 Directory Structure

```
customised_nodemcu/
├── include/            # Header files
├── lib/                # External libraries
├── src/                # Source code
│   └── main.cpp        # Main application logic
├── platformio.ini      # PlatformIO project configuration
```

---

## 🪰 Getting Started

### Prerequisites

* [PlatformIO](https://platformio.org/) installed.
* ESP8266-based NodeMCU board.

### Installation

1. **Clone the Repository**:

   ```bash
   git clone https://github.com/ziyaulhaq10/customised_nodemcu.git
   cd customised_nodemcu
   ```

2. **Open with PlatformIO**:

   Open the project directory in your preferred IDE with PlatformIO support (e.g., VS Code).

3. **Build and Upload**:

   Connect your NodeMCU board and upload the firmware:

   ```bash
   pio run --target upload
   ```

---

## ⚙️ Configuration

The device reads its configuration from a JSON file stored in EEPROM.

### Sample JSON Configuration

```json
{
  "hostname": "office",
  "title": "Home Control Panel",
  "pinconfig": {
    "D0": "FAN",
    "D1": "LIGHT",
    "D2": "PUMP"
  }
}
```

* **hostname**: Device's mDNS hostname.
* **title**: Title displayed on the web interface.
* **pinconfig**: Mapping of GPIO pins to their labels.

### Uploading Configuration

1. Access the web interface at `http://<hostname>.local/upload`.
2. Paste your JSON configuration into the provided textarea.
3. Submit the form to save the configuration and reboot the device.

---

## 🌐 Usage

* Navigate to `http://<hostname>.local/` in your web browser.
* The interface displays buttons for each configured pin.
* Click buttons to toggle the state of the corresponding GPIO pins.

---

## 📝 Notes

* Only pins specified in the `pinconfig` will appear on the interface.
* Ensure the JSON configuration is valid to prevent parsing errors.
* The interface uses raw literals for HTML content to optimize memory usage.

---

## 🤝 Contributing

Contributions are welcome! Please fork the repository and submit a pull request for any enhancements or bug fixes.

# testing