#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

#define PROJECT_NAME "SomfyWind"
#define PROJECT_VERSION "1.0.0"

// ESP8266 D2 / GPIO4. Change this only if your anemometer is wired elsewhere.
#define WIND_SENSOR_PIN D2

// Somfy wind sensor calibration. Verify CALIBRATION_FACTOR against a reference
// instrument before relying on measurements for automation.
#define PULSES_PER_REVOLUTION 8
#define CUP_RADIUS 0.090f
#define CALIBRATION_FACTOR 2.80f
#define PULSE_DEBOUNCE_MS 5

#define SERIAL_BAUDRATE 9600
#define SERIAL_UPDATE_INTERVAL 1000

// Set these before uploading. Empty values deliberately start setup AP mode.
// Never commit real credentials to this public repository.
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define WIFI_HOSTNAME "SomfyWind"
#define MDNS_HOSTNAME "somfywind"
#define WIFI_TIMEOUT 15000

// Change the AP password before using the device outside initial setup.
#define AP_SSID "SomfyWind-Setup"
#define AP_PASSWORD "somfywind-setup"

#define HTTP_PORT 80

#endif // CONFIG_H
