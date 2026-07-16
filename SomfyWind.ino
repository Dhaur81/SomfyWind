/*
 * ============================================================================
 * Project    : SomfyWind
 * File       : SomfyWind.ino
 * Version    : 0.5.1
 *
 * Description:
 * Main application.
 *
 * Changelog:
 * --------------------------------------------------------------------------
 * v0.5.1
 * - Added mDNS (somfywind.local)
 * - Improved WiFi diagnostics
 * - Cleaner Serial output
 * - Automatic hostname
 * - Prepared for WiFi reconnect
 *
 * v0.5.0
 * - WiFi Station
 * - Access Point fallback
 * - HTTP server
 * ============================================================================
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#include "Config.h"
#include "WindSensor.h"
#include "WebServer.h"

// ============================================================================
// Objects
// ============================================================================

WindSensor windSensor;
SomfyWebServer webServer;

unsigned long lastPrint = 0;

// ============================================================================
// WiFi
// ============================================================================

void connectWiFi()
{
    Serial.println();
    Serial.println("================================");
    Serial.println("WiFi");
    Serial.println("================================");

    WiFi.mode(WIFI_STA);

    // ESP8266 hostname
    WiFi.setHostname(WIFI_HOSTNAME);

    Serial.print("SSID : ");
    Serial.println(WIFI_SSID);

    Serial.println();
    Serial.print("Connecting ");

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    unsigned long startTime = millis();

    while (WiFi.status() != WL_CONNECTED &&
           millis() - startTime < WIFI_TIMEOUT)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.print("WiFi.status() = ");
    Serial.println(WiFi.status());

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println();
        Serial.println("WiFi connected");
        Serial.print("IP Address : ");
        Serial.println(WiFi.localIP());

        Serial.print("RSSI       : ");
        Serial.print(WiFi.RSSI());
        Serial.println(" dBm");

        if (MDNS.begin("somfywind"))
        {
            Serial.println("mDNS       : OK");
            Serial.println("URL        : http://somfywind.local");
        }
        else
        {
            Serial.println("mDNS       : FAILED");
        }

        return;
    }

    Serial.println();
    Serial.println("WiFi connection failed");
    Serial.println("Starting Access Point");

    WiFi.mode(WIFI_AP);

    WiFi.softAP(AP_SSID, AP_PASSWORD);

    Serial.print("AP SSID    : ");
    Serial.println(AP_SSID);

    Serial.print("AP Address : ");
    Serial.println(WiFi.softAPIP());

    Serial.println("URL        : http://192.168.4.1");
}

// ============================================================================

void setup()
{
    Serial.begin(SERIAL_BAUDRATE);

    Serial.println();
    Serial.println("================================");
    Serial.print(PROJECT_NAME);
    Serial.print(" ");
    Serial.println(PROJECT_VERSION);
    Serial.println("================================");

    windSensor.begin();

    connectWiFi();

    webServer.begin(&windSensor);

    Serial.println();
    Serial.println("HTTP server started");

    if (WiFi.getMode() == WIFI_STA)
    {
        Serial.print("Open : http://");
        Serial.println(WiFi.localIP());

        Serial.println("Open : http://somfywind.local");
    }
    else
    {
        Serial.println("Open : http://192.168.4.1");
    }

    Serial.println();
    Serial.println("System ready.");
    Serial.println();
}

// ============================================================================

void loop()
{
    windSensor.update();

    webServer.handleClient();

    // Handle mDNS requests
    if (WiFi.status() == WL_CONNECTED)
    {
        MDNS.update();
    }

    // Automatic reconnect
    static unsigned long lastReconnectAttempt = 0;

    if (WiFi.getMode() == WIFI_STA &&
        WiFi.status() != WL_CONNECTED &&
        millis() - lastReconnectAttempt > 30000)
    {
        lastReconnectAttempt = millis();

        Serial.println();
        Serial.println("WiFi lost - reconnecting...");

        WiFi.disconnect();
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    }

    // Serial diagnostics
    if (millis() - lastPrint >= SERIAL_UPDATE_INTERVAL)
    {
        lastPrint = millis();

      Serial.print("Wind: ");
      Serial.print(windSensor.getWindSpeed(), 2);
      Serial.print(" m/s");

      Serial.print(" | Gust 3s: ");
      Serial.print(windSensor.getWindGust3s(), 2);

      Serial.print(" | Avg10s: ");
      Serial.print(windSensor.getWindAverage10s(), 2);

      Serial.print(" | Avg2m: ");
      Serial.print(windSensor.getWindAverage2min(), 2);

      Serial.print(" | Max: ");
      Serial.print(windSensor.getMaxSinceStart(), 2);

      if (WiFi.status() == WL_CONNECTED)
{
    Serial.print(" | WiFi OK");
}
else
{
    Serial.print(" | WiFi LOST");
}

Serial.println();
    }
}