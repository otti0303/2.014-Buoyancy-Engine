// PUMPY ARDUINO UNO CPP SCRIPT FOR SENSORS

#include <Wire.h>
#include "MS5837.h"

MS5837 pressure_sensor;

const int thermistorPin1 = A0; // ADC pin for thermistor
const int thermistorPin2 = A1; // ADC pin for thermistor
const float seriesResistor = 10000.0; // 10kΩ resistor in series
const float nominalResistance = 100000.0; // Thermistor resistance at 25°C
const float nominalTemperature = 25.0; // In Celsius
const float bCoefficient = 3950.0; // B value from datasheet
const int adcMax = 1023; // Arduino Uno ADC is 10-bit
const float supplyVoltage = 5.0; // Arduino Uno runs at 5V

void setup() {
    Serial.begin(9600);
    delay(500);
    Serial.println("Starting...");

    Wire.begin(); // Use default I2C pins on Uno (A4 = SDA, A5 = SCL)
    Serial.println("Wire.begin done.");

    if (!pressure_sensor.init()) {
        Serial.println("Pressure sensor could not be initialized. Check wiring.");
        while (1);
    }

    pressure_sensor.setModel(MS5837::MS5837_02BA);
    pressure_sensor.setFluidDensity(997); // kg/m^3 for fresh water
    Serial.println("Pressure sensor initialized.");
}

void loop() {

    int adcValue1 = analogRead(thermistorPin1);
    float voltage1 = adcValue1 * supplyVoltage / adcMax;
    float resistance1 = seriesResistor * (supplyVoltage / voltage1 - 1);

    float tempK1 = 1.0 / (1.0 / (nominalTemperature + 273.15) + log(resistance1 / nominalResistance) / bCoefficient);
    float tempC1 = tempK1 - 273.15;

    int adcValue2 = analogRead(thermistorPin2);
    float voltage2 = adcValue2 * supplyVoltage / adcMax;
    float resistance2 = seriesResistor * (supplyVoltage / voltage2 - 1);

    float tempK2 = 1.0 / (1.0 / (nominalTemperature + 273.15) + log(resistance2 / nominalResistance) / bCoefficient);
    float tempC2 = tempK2 - 273.15;

    // PRESSURE SENSOR (internal, hull)
    pressure_sensor.read();

    Serial.print("TEMP1:");
    Serial.print(tempC1); // °C
    Serial.print(",TEMP2:");
    Serial.print(tempC2); // °C
    Serial.print(",PRESSURE:");
    Serial.print(pressure_sensor.pressure()); // hPa
    Serial.print(",DEPTH:");
    Serial.println(pressure_sensor.depth()); // m

    delay(200);
}