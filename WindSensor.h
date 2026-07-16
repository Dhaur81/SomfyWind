/*
 * ============================================================================
 * Project    : SomfyWind
 * File       : WindSensor.h
 * Version    : 0.6.0
 * Author     : ChatGPT
 *
 * Description:
 * Wind sensor class for Somfy anemometer.
 *
 * Changelog:
 * --------------------------------------------------------------------------
 * v0.6.0
 * - Added 120 second history buffer
 * - Added 3 second wind gust
 * - Added 10 second average
 * - Added 2 minute average
 * - Added maximum since startup
 * - Improved statistics architecture
 *
 * v0.5.0
 * - RPM calculation
 * - Total pulse counter
 * - Last pulse timer
 * ============================================================================
 */

#ifndef WIND_SENSOR_H
#define WIND_SENSOR_H

#include <Arduino.h>

// ============================================================================
// Wind history
// ============================================================================

#define WIND_HISTORY_SIZE 120

class WindSensor
{
public:
    WindSensor();

    void begin();
    void update();

    //=========================================================================
    // Current wind
    //=========================================================================

    float getWindSpeed() const;
    float getWindSpeedKmh() const;

    //=========================================================================
    // Wind statistics
    //=========================================================================

    float getWindGust3s() const;
    float getWindAverage10s() const;
    float getWindAverage2min() const;
    float getMaxSinceStart() const;

    //=========================================================================
    // Diagnostics
    //=========================================================================

    float getPulseRate() const;
    float getRevolutionsPerSecond() const;
    float getRPM() const;

    unsigned long getTotalPulses() const;
    unsigned long getLastPulseAge() const;

private:

    //=========================================================================
    // Interrupt
    //=========================================================================

    static void IRAM_ATTR isr();

    static volatile unsigned long pulseCount;
    static volatile unsigned long lastPulseMillis;

    //=========================================================================
    // Measurement
    //=========================================================================

    unsigned long lastPulseCount;
    unsigned long lastUpdateTime;

    float pulseRate;
    float revolutionsPerSecond;
    float rpm;
    float windSpeed;

    //=========================================================================
    // Statistics
    //=========================================================================

    float windHistory[WIND_HISTORY_SIZE];

    uint16_t historyIndex;
    uint16_t historyCount;

    float windGust3s;
    float windAverage10s;
    float windAverage2min;
    float maxSinceStart;

    //=========================================================================
    // Internal functions
    //=========================================================================

    void updateStatistics();
};

#endif // WIND_SENSOR_H