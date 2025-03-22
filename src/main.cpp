// main for the ideal system, not for testing
// i.e. many placeholder functions

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "fuel_control.h"
#include "vent_control.h"
#include "water_intake_control.h"
#include "sensors.h"
#include "config.h"

// PWM object
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

void setup() {
    Serial.begin(115200);

    initCAN(); // need to fix this func
    i2c_connect(); // placeholder function (fix)

    Wire.begin(I2C_SDA, I2C_SCL);

    // update / init all necessary I2C connections

    // PWM setup
    pwm.begin();
    pwm.setPWMFreq(PWM_FREQUENCY);

    // MOSFETs as which outputs ??
    // pinMode(MOSFET_FUEL, OUTPUT);
    // pinMode(MOSFET_VENT, OUTPUT);
    
    // resetCmds();

    // turn on LED once initialized (for debugging)

    Serial.println("ESP32-S3 Initialized!");
}

// put these in beginning of loop?? so they always reset back to false (but appropriate functions should still work)
bool ascent_cmd = false; // rise (fuel and ammonia reaction)
bool descent_cmd = false; // sink (gas vent, water intake)

bool abort_reaction = false; // stop fuel / amm pumps, vent gas and intake water
bool abort_descent = false; // stop gas vent and water intake pumps 

// somehow using laptop commands we will update these bools in the loop, keep checking if theyve been pressed
// ^ using the CAN BUS connection

void loop() {

    sendCANMessage(PRESSURE_SENSOR_ID); // Request pressure sensor data
    sendCANMessage(TEMP_SENSOR_ID);     // Request temperature sensor data
    receiveCANMessage();                // Receive sensor responses
    delay(1000);                         // Wait before next request

    // command(); // placeholder function, basically check if GUI has activated any commands

    // function / connection that checks if cmd bools above have been activated (turns them to true if so)

    if (ascent_cmd) {
        startFuelAmmDispensing();
        // reset ascent_cmd to false? where?
    }

    if (descent_cmd) {
        startGasVent(); // automatically starts water intake once gas vent stops
        // reset descent_cmd to false? where?
    }

    if (abort_reaction) {
        ABORT_fuel_amm();
        // reset abort_reaction cmd to false? where?
    }

    if (abort_descent) {
        ABORT_vent_and_intake();
        // reset abort_descent cmd to false? where?
    }

    delay(100);
}
