# 🐕 Sasha-Watch — Edge Environmental Monitor

> Built to protect my dog Sasha during 12-hour overnight shifts at a factory job in Arizona.
> If the AC fails, I need to know before she does.

## What It Does

Sasha-Watch is a real-time environmental monitoring system that tracks temperature and
humidity in my apartment using an ESP32-S3 microcontroller and Si7021 sensor. Data is
logged to a time-series database on a local single-board computer, visualized in a
Grafana dashboard accessible from my phone, and triggers an instant Telegram alert if
conditions become dangerous.

## Why It Matters

Arizona summers. Unreliable swamp coolers. A dog home alone. The legal temperature
limit for rental units in Arizona is 82°F for A/C, but my current complex runs swamp coolers.
This means they have to take action within 5 days of the temp hitting 86 degrees F.
 
This system ensures I have remote visibility and an early warning — not just "is it hot," 
but "how fast is it heating up."

This is the same or similar to the monitoring and alerting loops used in data center NOC environments —
threshold detection, time-series logging, remote dashboards, and automated alerting.

### Hardware

| Component | Role |
|---|---|
| ESP32-S3 Metro | Microcontroller / edge device |
| SI7021 | Temperature and humidity sensor (STEMMA QT / I2C) |
| Orange Pi 5B | Local server — database, dashboard, automation |

### Software Stack

| Layer | Technology |
|---|---|
| Firmware | Arduino IDE (C++) |
| Database | InfluxDB (time-series) |
| Dashboard | Grafana |
| Automation | n8n |
| Alerting | Telegram Bot |
| Version Control | Git / GitHub |
| OS (SBC) | Joshua Riggs Ubuntu (Orange Pi 5B) |

### Alert Thresholds

| Level | Condition |
|---|---|
| ⚠️ Warning | Temperature ≥ 80°F |
| 🔴 Critical | Temperature ≥ 83°F |

## Project Structure
```
sasha-watch/
├── firmware/
│   └── src/          # ESP32 Arduino sketches
├── docs/             # Wiring diagrams, setup guides, notes
├── dashboards/       # Grafana dashboard JSON exports
├── scripts/          # Automation and helper scripts
└── README.md
```

### Roadmap

- [x] Repo initialized and structured
- [x] SI7021 sensor reading via Arduino IDE
- [x] Serial output confirmed
- [x] WiFi + data transmission to Orange Pi
- [x] InfluxDB installed and receiving data
- [x] Grafana dashboard live
- [ ] Telegram alerting via n8n
- [ ] Low humidity alert for Solomon (below 40%)
- [ ] n8n automation configured
- [ ] ADXL335 vibration sensor integration (Phase 2)

### Author

Kyle Hebenheimer — [@millennial-coding](https://github.com/millennial-coding)
