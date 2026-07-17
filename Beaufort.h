#ifndef BEAUFORT_H
#define BEAUFORT_H

#include <Arduino.h>

class Beaufort
{
public:
    static uint8_t number(float metersPerSecond);
    static const char* description(float metersPerSecond);
};

#endif // BEAUFORT_H
