// UPLOAD TO ESP32
// upload this script to the ESP32 using arduino IDE, then quit arduino (such that port is not open/being used by arduino IDE)
// then run user_GUI.py on laptop terminal which will connect to ESP32 port using python

#define ALUMINUM_PUMP 16 
#define AMMONIA_PUMP 17 
#define GAS_VENT_PUMP 18
#define WATER_INTAKE_PUMP 19

// THERMISTOR DEFINITIONS
#define THERMISTOR_1 15 // UPDATE BASED ON WHICH PIN SECOND THERMISTOR (EXTERNAL TANK TEMP) IS CONNECTED THROUGH
#define THERMISTOR_2 34 
const float seriesResistor = 10000.0; // 10kΩ resistor in series
const float nominalResistance = 100000.0; // Thermistor resistance at 25°C
const float nominalTemperature = 25.0; // In Celsius
const float bCoefficient = 3950.0; // B value from datasheet
const int adcMax = 4095; // ESP32 ADC is 12-bit
const float supplyVoltage = 5;

// PRESSURE SENSOR DEFINITIONS
#include <Wire.h>
#include "MS5837.h"
MS5837 pressure_sensor;

void setup() {
    pinMode(ALUMINUM_PUMP, OUTPUT);
    pinMode(AMMONIA_PUMP, OUTPUT);
    pinMode(GAS_VENT_PUMP, OUTPUT);
    pinMode(WATER_INTAKE_PUMP, OUTPUT);

    pinMode(THERMISTOR_1, INPUT);
    pinMode(THERMISTOR_2, INPUT);

    digitalWrite(AMMONIA_PUMP, LOW);
    digitalWrite(ALUMINUM_PUMP, LOW);
    digitalWrite(GAS_VENT_PUMP, LOW);
    digitalWrite(WATER_INTAKE_PUMP, LOW);

    Serial.begin(115200);

    Wire.begin(21, 22); // ESP32 I2C pins
    Serial.println("I2C connection complete.");

    if (!pressure_sensor.init()) {
        Serial.println("Pressure sensor could not be initialized. Check I2C wiring.");
        while (1);
    }

    pressure_sensor.setModel(MS5837::MS5837_02BA);
    pressure_sensor.setFluidDensity(997); // kg/m^3 for fresh water
    Serial.println("Pressure sensor initialized.");
}

unsigned long ascendStartTime = 0;
const unsigned long ascendDuration = 3000;

const unsigned long ammoniaDuration = 3000;
const unsigned long aluminumDuration = 3000;

unsigned long ventStartTime = 0;
const unsigned long ventDuration = 3000;

unsigned long intakeStartTime = 0;
const unsigned long intakeDuration = 3000;

unsigned long ammoniaHighStartTime = 0;
const unsigned long ammoniaTimeHigh = 200;

unsigned long ammoniaLowStartTime = 0;
const unsigned long ammoniaTimeLow = 800;

unsigned long aluminumHighStartTime = 0;
const unsigned long aluminumTimeHigh = 200;

unsigned long aluminumLowStartTime = 0;
const unsigned long aluminumTimeLow = 800;

bool ascendInProgress = false;
bool ventInProgress = false;
bool intakeInProgress = false;

bool ammoniaHigh = false;
bool aluminumHigh = false;

String last_comm = "";

void loop() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');  // Read the incoming command
        command.trim();

        if (last_comm != command) {

            if (command == "ASCEND") {
                digitalWrite(AMMONIA_PUMP, HIGH);
                ammoniaHighStartTime = millis();
                ammoniaHigh = true;
                ammoniaLowStartTime = 0;

                digitalWrite(ALUMINUM_PUMP, HIGH);
                aluminumHighStartTime = millis();
                aluminumHigh = true;
                aluminumLowStartTime = 0;

                ascendStartTime = millis(); 
                ascendInProgress = true;
                last_comm = "ASCEND";
            } 
            
            else if (command == "DESCEND") {
                digitalWrite(GAS_VENT_PUMP, HIGH);

                ventStartTime = millis();
                ventInProgress = true;
                last_comm = "DESCEND";
            } 
            
            else if (command == "ABORT_ASCENT") {
                digitalWrite(AMMONIA_PUMP, LOW);
                digitalWrite(ALUMINUM_PUMP, LOW);

                ascendInProgress = false;
                last_comm = "ABORT_ASCENT";

                Serial.println("TASK COMPLETED");
                last_comm = "RESET";
            } 
            
            else if (command == "ABORT_DESCENT") {
                digitalWrite(GAS_VENT_PUMP, LOW);
                digitalWrite(WATER_INTAKE_PUMP, LOW);

                ventInProgress = false;
                intakeInProgress = false;
                last_comm = "ABORT_DESCENT";

                Serial.println("TASK COMPLETED");
                last_comm = "RESET";
            } 

            // don't need this command ?
            else if (command == "RESET") {
                // digitalWrite(AMMONIA_PUMP, LOW);
                // digitalWrite(ALUMINUM_PUMP, LOW);
                // digitalWrite(GAS_VENT_PUMP, LOW);
                // digitalWrite(WATER_INTAKE_PUMP, LOW);

                last_comm = "RESET";
            }

    }
}

        if (ascendInProgress) {

            if (millis() - ascendStartTime >= ammoniaDuration) {
                digitalWrite(AMMONIA_PUMP, LOW);
                
                if (ammoniaDuration >= aluminumDuration) {
                    ascendInProgress = false;
                    Serial.println("TASK COMPLETED");
                    last_comm = "RESET";
                }
            }

            if (millis() - ascendStartTime >= aluminumDuration) {
                digitalWrite(ALUMINUM_PUMP, LOW);
    
                if (aluminumDuration >= ammoniaDuration) {
                    ascendInProgress = false;
                    Serial.println("TASK COMPLETED");
                    last_comm = "RESET";
                }
            }
        }

        if (ascendInProgress) {

            if (ammoniaHigh && (millis() - ammoniaHighStartTime >= ammoniaTimeHigh)) {
                digitalWrite(AMMONIA_PUMP, LOW);
                ammoniaHigh = false;
            }

            if (!ammoniaHigh && (millis() - ammoniaLowStartTime >= ammoniaTimeLow)) {
                digitalWrite(AMMONIA_PUMP, HIGH);
                ammoniaHigh = true;
            }

            if (aluminumHigh && (millis() - aluminumHighStartTime >= aluminumTimeHigh)) {
                digitalWrite(ALUMINUM_PUMP, LOW);
                aluminumHigh = false;
            }

            if (!aluminumHigh && (millis() - aluminumLowStartTime >= aluminumTimeLow)) {
                digitalWrite(ALUMINUM_PUMP, HIGH);
                aluminumHigh = true;
            }

        }

        if ((millis() - ventStartTime >= ventDuration) && ventInProgress) {
            digitalWrite(GAS_VENT_PUMP, LOW);

            ventInProgress = false;

            digitalWrite(WATER_INTAKE_PUMP, HIGH);

            intakeStartTime = millis();
            intakeInProgress = true;
        }

        if ((millis() - intakeStartTime >= intakeDuration) && intakeInProgress) {
            digitalWrite(WATER_INTAKE_PUMP, LOW);

            intakeInProgress = false;
            Serial.println("TASK COMPLETED");
            last_comm = "RESET";
        }    


        // THERMISTOR 1 (external, in tank)
        int adcValue1 = analogRead(THERMISTOR_1);
        float voltage1 = adcValue1 * supplyVoltage / adcMax;
        float resistance1 = seriesResistor * (supplyVoltage / voltage1 - 1);
        float tempK1 = 1.0 / (1.0 / (nominalTemperature + 273.15) + log(resistance1 / nominalResistance) / bCoefficient);
        float tempC1 = tempK1 - 273.15;

        // THERMISTOR 2 (internal, in bladder)
        int adcValue2 = analogRead(THERMISTOR_2);
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
        Serial.println(pressure_sensor.depth()); // m?
}