/*
 * ============================================================================
 * Project    : SomfyWind
 * File       : Config.h
 * Version    : 0.5.1
 * Author     : ChatGPT
 *
 * Description:
 * Global configuration for SomfyWind.
 *
 * Changelog:
 * --------------------------------------------------------------------------
 * v0.5.1
 * - Added mDNS hostname
 * - Added HTTP port
 * - Serial baudrate fixed to 9600
 *
 * v0.5.0
 * - WiFi support
 * - Access Point fallback
 * ============================================================================
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ============================================================================
// Project
// ============================================================================

#define PROJECT_NAME        "SomfyWind"
#define PROJECT_VERSION     "0.5.1"

// ============================================================================
// Pin configuration
// ============================================================================

#define WIND_SENSOR_PIN D2

// ============================================================================
// Wind sensor
// ============================================================================

#define PULSES_PER_REVOLUTION   8
#define CUP_RADIUS              0.090f
#define CALIBRATION_FACTOR      2.80f

// ============================================================================
// Serial
// ============================================================================

#define SERIAL_BAUDRATE             9600
#define SERIAL_UPDATE_INTERVAL      1000

// ============================================================================
// WiFi
// ============================================================================

#define WIFI_SSID          "NETGEAR15"
#define WIFI_PASSWORD      "I3erget!"

#define WIFI_HOSTNAME      "SomfyWind"
#define MDNS_HOSTNAME      "somfywind"

#define WIFI_TIMEOUT       15000

// ============================================================================
// Access Point
// ============================================================================

#define AP_SSID            "SomfyWind"
#define AP_PASSWORD        "somfywind"

// ============================================================================
// HTTP
// ============================================================================

#define HTTP_PORT          80

#endif