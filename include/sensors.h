#ifndef SENSORS_H
#define SENSORS_H

#include "driver/twai.h"
#include <Arduino.h>

// Define CAN message IDs for sensors
#define PRESSURE_SENSOR_ID  0x100
#define TEMP_SENSOR_ID      0x101

// Function declarations
void initCAN();
void sendCANMessage(uint32_t sensorID);
float receiveCANMessage();
float getHullPressure();
float getCurrentTemperature();

#endif // SENSORS_H