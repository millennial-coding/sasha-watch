// sasha_watch_sensor.ino
// Monitors temperature and humidity using SI7021
// Logs to InfluxDB on Orange Pi 5B
// Protects Sasha during overnight shifts

#include <esp_task_wdt.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <ArduinoOTA.h>
#include <Wire.h>
#include <Adafruit_Si7021.h>
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include "secrets.h"

// WiFi and sensor objects
WiFiMulti wifiMulti;
Adafruit_Si7021 si7021 = Adafruit_Si7021();

// InfluxDB client and data point
InfluxDBClient influxClient(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);
Point sensorData("environment");

// Tracks the last time a sensor reading was taken
// unsigned long holds milliseconds since boot — needs to be large
// enough to store values up to ~50 days worth of milliseconds
unsigned long lastReadingMillis = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 5000) delay(10);

  Serial.println("Sasha-Watch booting...");

  Wire.begin();

  if (!si7021.begin()) {
    Serial.println("ERROR: Si7021 not found. Check wiring.");
    while (1) delay(100);
  }
  Serial.println("Si7021 OK");

  wifiMulti.addAP(WIFI_SSID_HOME, WIFI_PASS_HOME);
  wifiMulti.addAP(WIFI_SSID_HOT, WIFI_PASS_HOT);

  Serial.print("Connecting to WiFi");
  while (wifiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  ArduinoOTA.setHostname("sasha-watch-s3");
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    esp_task_wdt_reset();
});
  ArduinoOTA.begin();
  Serial.println("OTA ready");

  sensorData.addTag("device", "esp32-s3-metro");
  sensorData.addTag("location", "apartment");

  if (influxClient.validateConnection()) {
    Serial.print("InfluxDB connected: ");
    Serial.println(influxClient.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(influxClient.getLastErrorMessage());
  }

  // Watchdog timer — reboots ESP32 if loop freezes for 60 seconds
  const esp_task_wdt_config_t wdt_config = {
      .timeout_ms = 120000,
      .idle_core_mask = 0,
      .trigger_panic = true
  };
  esp_task_wdt_init(&wdt_config);
  esp_task_wdt_add(NULL);
  Serial.println("Watchdog timer started (60s timeout)");
}

void loop() {
  // Reset watchdog — prove we're alive every loop iteration
  esp_task_wdt_reset();

  // Always handle OTA first — never block it
  ArduinoOTA.handle();

  if (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("WiFi disconnected, reconnecting...");
    delay(1000);
    return;
  }

  // Non-blocking 30 second interval
  // millis() returns ms since boot — no blocking, loop runs freely
  unsigned long currentMillis = millis();
  if (currentMillis - lastReadingMillis >= 30000) {
    lastReadingMillis = currentMillis;

    float tempC = si7021.readTemperature();
    float tempF = (tempC * 1.8) + 32.0;
    float humidity = si7021.readHumidity();

    Serial.print("Temp: ");
    Serial.print(tempF);
    Serial.print(" F  Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    sensorData.clearFields();
    sensorData.addField("temperature_f", tempF);
    sensorData.addField("humidity", humidity);

    if (!influxClient.writePoint(sensorData)) {
      Serial.print("InfluxDB write failed: ");
      Serial.println(influxClient.getLastErrorMessage());
    }
  }
}
