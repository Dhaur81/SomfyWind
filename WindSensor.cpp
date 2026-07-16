/*
 * ============================================================================
 * Project    : SomfyWind
 * File       : WindSensor.cpp
 * Version    : 0.5.1
 * Author     : ChatGPT
 *
 * Description:
 * Wind sensor implementation for Somfy anemometer.
 *
 * Changelog:
 * --------------------------------------------------------------------------
 * v0.5.0
 * - Added RPM calculation
 * - Added total pulse counter
 * - Added last pulse timer
 * - Prepared for diagnostics page
 *
 * v0.4.1
 * - Stable 1 second measurement window
 *
 * v0.4.0
 * - Initial version
 * ============================================================================
 */

#include "WindSensor.h"
#include "Config.h"

volatile unsigned long WindSensor::pulseCount = 0;
volatile unsigned long WindSensor::lastPulseMillis = 0;

void IRAM_ATTR WindSensor::isr()

{
    pulseCount++;
    lastPulseMillis = millis();
}

WindSensor::WindSensor()
{
    lastPulseCount = 0;
    lastUpdateTime = 0;

    pulseRate = 0.0f;
    revolutionsPerSecond = 0.0f;
    rpm = 0.0f;
    windSpeed = 0.0f;

    historyIndex = 0;
    historyCount = 0;

    windGust3s = 0.0f;
    windAverage10s = 0.0f;
    windAverage2min = 0.0f;
    maxSinceStart = 0.0f;

    for (uint16_t i = 0; i < WIND_HISTORY_SIZE; i++)
    {
        windHistory[i] = 0.0f;
    }
}

void WindSensor::begin()
{
    pinMode(WIND_SENSOR_PIN, INPUT_PULLUP);

    attachInterrupt(
        digitalPinToInterrupt(WIND_SENSOR_PIN),
        WindSensor::isr,
        FALLING);

    lastUpdateTime = millis();
}

void WindSensor::update()
{
    unsigned long now = millis();
    unsigned long elapsed = now - lastUpdateTime;

    // Beräkna en gång per sekund
    if (elapsed < 1000)
        return;

    noInterrupts();
    unsigned long pulses = pulseCount;
    interrupts();

    unsigned long pulseDelta = pulses - lastPulseCount;

    float elapsedSeconds = elapsed / 1000.0f;

    pulseRate = pulseDelta / elapsedSeconds;

    revolutionsPerSecond =
        pulseRate / PULSES_PER_REVOLUTION;

    rpm = revolutionsPerSecond * 60.0f;

    float circumference =
        2.0f * PI * CUP_RADIUS;

    windSpeed =
revolutionsPerSecond *
    circumference *
    CALIBRATION_FACTOR;

//----------------------------------------------------
// Save current wind value
//----------------------------------------------------

windHistory[historyIndex] = windSpeed;

historyIndex++;

if (historyIndex >= WIND_HISTORY_SIZE)
{
    historyIndex = 0;
}

if (historyCount < WIND_HISTORY_SIZE)
{
    historyCount++;
}

updateStatistics();

//----------------------------------------------------

lastPulseCount = pulses;
lastUpdateTime = now;
}

float WindSensor::getPulseRate() const
{
    return pulseRate;
}

float WindSensor::getRevolutionsPerSecond() const
{
    return revolutionsPerSecond;
}

float WindSensor::getRPM() const
{
    return rpm;
}

float WindSensor::getWindSpeed() const
{
    return windSpeed;
}

float WindSensor::getWindSpeedKmh() const
{
    return windSpeed * 3.6f;
}

unsigned long WindSensor::getTotalPulses() const
{
    noInterrupts();
    unsigned long pulses = pulseCount;
    interrupts();

    return pulses;
}

unsigned long WindSensor::getLastPulseAge() const
{
    noInterrupts();
    unsigned long last = lastPulseMillis;
    interrupts();

    if (last == 0)
        return 0;

    return millis() - last;
}

float WindSensor::getWindGust3s() const
{
    return windGust3s;
}

float WindSensor::getWindAverage10s() const
{
    return windAverage10s;
}

float WindSensor::getWindAverage2min() const
{
    return windAverage2min;
}

float WindSensor::getMaxSinceStart() const
{
    return maxSinceStart;
}

void WindSensor::updateStatistics()
{
    //----------------------------------------------------------
    // Max sedan start
    //----------------------------------------------------------

    if (windSpeed > maxSinceStart)
    {
        maxSinceStart = windSpeed;
    }

    //----------------------------------------------------------
    // 3 sekunders medel (vindby)
    //----------------------------------------------------------

    windGust3s = 0.0f;

    uint16_t samples = min((uint16_t)3, historyCount);

    if (samples > 0)
    {
        float sum = 0.0f;

        for (uint16_t i = 0; i < samples; i++)
        {
            int16_t index = historyIndex - 1 - i;

            if (index < 0)
                index += WIND_HISTORY_SIZE;

            sum += windHistory[index];
        }

        windGust3s = sum / samples;
    }

    //----------------------------------------------------------
    // 10 sekunders medel
    //----------------------------------------------------------

    windAverage10s = 0.0f;

    samples = min((uint16_t)10, historyCount);

    if (samples > 0)
    {
        float sum = 0.0f;

        for (uint16_t i = 0; i < samples; i++)
        {
            int16_t index = historyIndex - 1 - i;

            if (index < 0)
                index += WIND_HISTORY_SIZE;

            sum += windHistory[index];
        }

        windAverage10s = sum / samples;
    }

    //----------------------------------------------------------
    // 2 minuters medel
    //----------------------------------------------------------

    windAverage2min = 0.0f;

    if (historyCount > 0)
    {
        float sum = 0.0f;

        for (uint16_t i = 0; i < historyCount; i++)
        {
            sum += windHistory[i];
        }

        windAverage2min = sum / historyCount;
    }
}