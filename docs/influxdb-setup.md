# InfluxDB Setup Guide

## Overview
InfluxDB 2.x is used as the time-series database for Sasha-Watch.
It runs as a systemd service on the Orange Pi 5B and receives data
from the ESP32-S3 via HTTP on port 8086.

## Installation

Added the official InfluxData repository and installed via apt:
```bash
sudo gpg --keyserver keyserver.ubuntu.com --recv-keys DA61C26A0585BD3B
sudo gpg --export DA61C26A0585BD3B | sudo tee /etc/apt/trusted.gpg.d/influxdata-archive_compat.gpg > /dev/null
echo "deb [signed-by=/etc/apt/trusted.gpg.d/influxdata-archive_compat.gpg] https://repos.influxdata.com/debian stable main" | sudo tee /etc/apt/sources.list.d/influxdata.list
sudo apt update
sudo apt install influxdb2
```

## Service Management
```bash
sudo systemctl start influxdb      # start the service
sudo systemctl stop influxdb       # stop the service
sudo systemctl restart influxdb    # restart the service
sudo systemctl status influxdb     # check if running
```

InfluxDB is enabled to start automatically on boot.

## Configuration

- **URL:** http://192.168.0.38:8086
- **Organization:** sasha-watch
- **Bucket:** environment
- **Web UI:** http://localhost:8086

## API Tokens

Two tokens are configured:

| Token | Purpose | Stored |
|---|---|---|
| Operator Token | Full admin access | Bitwarden |
| ESP32 Write Token | Write-only to environment bucket | Bitwarden + secrets.h |

## Data Schema

Measurement: `environment`

| Field | Type | Description |
|---|---|---|
| temperature_f | float | Temperature in Fahrenheit |
| humidity | float | Relative humidity percentage |

Tags:
- `device` — esp32-s3-metro
- `location` — apartment

## Verifying Data

Log into the web UI at http://localhost:8086 and use
Data Explorer to query the environment bucket.