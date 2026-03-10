# Firmware Setup Guide

## Overview
The ESP32-S3 Metro runs a continuous monitoring sketch written in Arduino (C++).
It reads temperature and humidity from the SI7021 sensor every 30 seconds and
writes the data to InfluxDB on the Orange Pi 5B over WiFi. OTA (Over The Air)
updates allow wireless firmware flashing without a USB cable.

## Hardware
- Adafruit Metro ESP32-S3
- Adafruit SI7021 Temperature & Humidity Sensor (I2C)
- Connected via STEMMA QT (JST SH 4-pin) — Adafruit's plug-and-play I2C
  connector. No soldering or manual wiring required. Plug and go.

## Dependencies
Install these libraries in Arduino IDE via Tools → Manage Libraries:

| Library | Version | Purpose |
|---|---|---|
| Adafruit Si7021 Library | 1.5.3 | SI7021 sensor driver |
| ESP8266 Influxdb | 3.13.2 | InfluxDB HTTP client |
| ArduinoOTA | built-in | Wireless firmware updates |
| WiFiMulti | built-in | Multi-network WiFi support |

## File Structure
```
firmware/src/sasha_watch_sensor/
├── sasha_watch_sensor.ino   # Main sketch
└── secrets.h                # WiFi and API credentials (never committed)
```

## secrets.h
Create this file manually — it is gitignored and never committed:
```cpp
#define WIFI_SSID_HOME  "your_home_ssid"
#define WIFI_PASS_HOME  "your_home_password"
#define WIFI_SSID_HOT   "your_hotspot_ssid"
#define WIFI_PASS_HOT   "your_hotspot_password"

#define INFLUXDB_URL    "http://192.168.0.38:8086"
#define INFLUXDB_TOKEN  "your_esp32_write_token"
#define INFLUXDB_ORG    "sasha-watch"
#define INFLUXDB_BUCKET "environment"
```

Tokens are stored in Bitwarden.

## Flashing via USB (first time or recovery)
```bash
arduino-cli compile --fqbn esp32:esp32:adafruit_metro_esp32s3 ~/sasha-watch/firmware/src/sasha_watch_sensor
arduino-cli upload -v --fqbn esp32:esp32:adafruit_metro_esp32s3 --port /dev/ttyACM0 ~/sasha-watch/firmware/src/sasha_watch_sensor
```

## Flashing via OTA (normal workflow)
1. Open Arduino IDE
2. Tools → Port → select 192.168.0.66 (or current IP)
3. Click Upload

Find the ESP32's current IP with:
```bash
nmap -sn 192.168.0.0/24
```

## Key Design Decisions

### Non-blocking loop with millis()
The sketch uses `millis()` instead of `delay()` for the 30 second reading
interval. This keeps the loop running freely so OTA requests are always
handled. `delay()` would freeze the board and make OTA unreliable.

### Watchdog Timer
A 120 second hardware watchdog reboots the ESP32 automatically if the loop
ever freezes — for example if WiFi hangs indefinitely. The watchdog is
reset every loop iteration and during OTA transfers via a progress callback.

### WiFiMulti
Registers both home WiFi and phone hotspot. If home WiFi drops, the board
automatically connects to the hotspot. Useful when working from different
locations or during router outages.

### Credential Security
All credentials live in secrets.h which is gitignored. Never commit
API tokens or passwords to a public repository.

## Board Settings (Arduino IDE)
- Board: Adafruit Metro ESP32-S3
- USB Mode: USB-OTG (TinyUSB)
- PSRAM: OPI PSRAM
- Partition Scheme: TinyUF2 16MB
- CPU Frequency: 240MHz