#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>

extern bool SerialDebugEnabled;

#define debug_print(...)   do { if (SerialDebugEnabled) Serial.print(__VA_ARGS__); } while(0)
#define debug_println(...) do { if (SerialDebugEnabled) Serial.println(__VA_ARGS__); } while(0)
#define debug_printf(...)  do { if (SerialDebugEnabled) Serial.printf(__VA_ARGS__); } while(0)

#endif  