/*
 * ============================================================================
 * Project    : SomfyWind
 * File       : WebServer.h
 * Version    : 0.5.0
 * Author     : ChatGPT
 *
 * Description:
 * Embedded HTTP server for SomfyWind.
 *
 * Changelog:
 * --------------------------------------------------------------------------
 * v0.5.0
 * - Initial implementation
 * - Live wind data
 * - Diagnostics support
 * ============================================================================
 */

#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Arduino.h>

class WindSensor;

class SomfyWebServer
{
public:
    SomfyWebServer();

    void begin(WindSensor* sensor);
    void handleClient();

private:
    WindSensor* _sensor;

    void handleRoot();
    void handleStats();
    void handleCalibration();
    void handleSettings();
    void handleDiagnostics();
    void handleNotFound();
};

#endif // WEBSERVER_H
