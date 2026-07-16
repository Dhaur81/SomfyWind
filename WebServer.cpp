/*
 * ============================================================================
 * Project    : SomfyWind
 * File       : WebServer.cpp
 * Version    : 0.5.1
 * Author     : ChatGPT
 *
 * Description:
 * Embedded HTTP server for SomfyWind.
 *
 * Changelog:
 * --------------------------------------------------------------------------
 * v0.5.1
 * - Added project version
 * - Added hostname
 * - Added WiFi information
 * - Improved mobile layout
 * - Added diagnostics
 *
 * v0.5.0
 * - Initial version
 * ============================================================================
 */

#include "WebServer.h"
#include "Config.h"
#include "WindSensor.h"

#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

ESP8266WebServer server(HTTP_PORT);

// ============================================================================

SomfyWebServer::SomfyWebServer()
{
    _sensor = nullptr;
}

// ============================================================================

void SomfyWebServer::begin(WindSensor *sensor)
{
    _sensor = sensor;

    server.on("/", [this]()
              { handleRoot(); });

    server.on("/stats", [this]()
              { handleStats(); });

    server.on("/calibration", [this]()
              { handleCalibration(); });

    server.on("/settings", [this]()
              { handleSettings(); });

    server.on("/diagnostics", [this]()
              { handleDiagnostics(); });

    server.onNotFound([this]()
                      { handleNotFound(); });

    server.begin();
}

// ============================================================================

void SomfyWebServer::handleClient()
{
    server.handleClient();
}

// ============================================================================

void SomfyWebServer::handleRoot()
{
    String html;

    html += "<!DOCTYPE html>";
    html += "<html>";
    html += "<head>";
    html += "<meta charset='utf-8'>";
    html += "<meta name='viewport' content='width=device-width,initial-scale=1'>";
    html += "<meta http-equiv='refresh' content='2'>";

    html += "<title>";
    html += PROJECT_NAME;
    html += "</title>";

    html += "<style>";
    html += "body{font-family:Arial;background:#ececec;margin:15px;}";
    html += ".card{background:#fff;border-radius:12px;padding:20px;"
            "max-width:520px;margin:auto;box-shadow:0 2px 10px #bbb;}";
    html += "h1{text-align:center;margin:0;}";
    html += ".version{text-align:center;color:#888;margin-bottom:25px;}";
    html += ".value{font-size:72px;font-weight:bold;text-align:center;}";
    html += ".unit{text-align:center;font-size:28px;color:#666;}";
    html += "table{width:100%;margin-top:20px;}";
    html += "td{padding:7px;}";
    html += "hr{margin:20px 0;}";
    html += "a{display:block;padding:8px 0;font-size:20px;text-decoration:none;}";
    html += "</style>";

    html += "</head><body>";

    html += "<div class='card'>";

    html += "<h1>";
    html += PROJECT_NAME;
    html += "</h1>";

    html += "<div class='version'>";
    html += PROJECT_VERSION;
    html += "</div>";

    html += "<div class='value'>";
    html += String(_sensor->getWindSpeed(), 1);
    html += "</div>";

    html += "<div class='unit'>m/s</div>";

    html += "<hr>";

    html += "<table>";

    html += "<tr><td>Vind</td><td>";
    html += String(_sensor->getWindSpeedKmh(), 1);
    html += " km/h</td></tr>";

    html += "<tr><td>RPM</td><td>";
    html += String(_sensor->getRPM(), 0);
    html += "</td></tr>";

    html += "<tr><td>Pulser/s</td><td>";
    html += String(_sensor->getPulseRate(), 1);
    html += "</td></tr>";

    html += "<tr><td>Varv/s</td><td>";
    html += String(_sensor->getRevolutionsPerSecond(), 2);
    html += "</td></tr>";

    html += "</table>";

    html += "<hr>";

    if (WiFi.status() == WL_CONNECTED)
    {
        html += "<p>🟢 WiFi anslutet</p>";
        html += "<p>";
        html += WiFi.localIP().toString();
        html += "</p>";
        html += "<p>";
        html += MDNS_HOSTNAME;
        html += ".local</p>";
    }
    else
    {
        html += "<p>🟠 Access Point</p>";
        html += "<p>";
        html += WiFi.softAPIP().toString();
        html += "</p>";
    }

    html += "<hr>";

    html += "<a href='/stats'>📊 Statistik</a>";
    html += "<a href='/calibration'>🎯 Kalibrering</a>";
    html += "<a href='/settings'>⚙ Inställningar</a>";
    html += "<a href='/diagnostics'>🛠 Diagnostik</a>";

    html += "</div>";
    html += "</body>";
    html += "</html>";

    server.send(200, "text/html", html);
}

// ============================================================================

void SomfyWebServer::handleStats()
{
    server.send(200,
                "text/html",
                "<h1>Statistik</h1>"
                "<p>Kommer i version 1.0.0</p>"
                "<p><a href='/'>Tillbaka</a></p>");
}

// ============================================================================

void SomfyWebServer::handleCalibration()
{
    server.send(200,
                "text/html",
                "<h1>Kalibrering</h1>"
                "<p>Kommer i version 1.0.0</p>"
                "<p><a href='/'>Tillbaka</a></p>");
}

// ============================================================================

void SomfyWebServer::handleSettings()
{
    String html;

    html += "<h1>Inställningar</h1>";

    html += "<p><b>SSID:</b> ";
    html += WIFI_SSID;
    html += "</p>";

    html += "<p><b>Hostname:</b> ";
    html += WIFI_HOSTNAME;
    html += "</p>";

    html += "<p><b>Kalibreringsfaktor:</b> ";
    html += String(CALIBRATION_FACTOR, 2);
    html += "</p>";

    html += "<p><a href='/'>Tillbaka</a></p>";

    server.send(200, "text/html", html);
}

// ============================================================================

void SomfyWebServer::handleDiagnostics()
{
    String html;

    html += "<h1>Diagnostik</h1>";

    html += "<table>";

    html += "<tr><td>Firmware</td><td>";
    html += PROJECT_VERSION;
    html += "</td></tr>";

    html += "<tr><td>Vind</td><td>";
    html += String(_sensor->getWindSpeed(), 2);
    html += " m/s</td></tr>";

    html += "<tr><td>Pulser/s</td><td>";
    html += String(_sensor->getPulseRate(), 2);
    html += "</td></tr>";

    html += "<tr><td>Varv/s</td><td>";
    html += String(_sensor->getRevolutionsPerSecond(), 2);
    html += "</td></tr>";

    html += "<tr><td>RPM</td><td>";
    html += String(_sensor->getRPM(), 0);
    html += "</td></tr>";

    html += "<tr><td>Totala pulser</td><td>";
    html += _sensor->getTotalPulses();
    html += "</td></tr>";

    html += "<tr><td>Senaste puls</td><td>";
    html += String(_sensor->getLastPulseAge() / 1000.0f, 2);
    html += " s</td></tr>";

    html += "<tr><td>Fri RAM</td><td>";
    html += ESP.getFreeHeap();
    html += " byte</td></tr>";

    html += "<tr><td>RSSI</td><td>";
    html += WiFi.RSSI();
    html += " dBm</td></tr>";

    html += "<tr><td>IP</td><td>";
    html += WiFi.localIP().toString();
    html += "</td></tr>";

    html += "<tr><td>Uptime</td><td>";
    html += String(millis() / 1000);
    html += " s</td></tr>";

    html += "</table>";

    html += "<p><a href='/'>Tillbaka</a></p>";

    server.send(200, "text/html", html);
}

// ============================================================================

void SomfyWebServer::handleNotFound()
{
    server.send(404, "text/plain", "404 Not Found");
}