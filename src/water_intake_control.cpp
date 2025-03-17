#include "water_intake_control.h"
#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include "config.h"

extern Adafruit_PWMServoDriver pwm;

// use / define MOSFET to drive this pump??

// set a certain appropriate duty cycle ... ask chem team for numbers
// calculate duty cycle based on pumps (flow rate / voltages)
#define WATER_PUMP_DUTY_CYCLE = 0.5

// WANT TO MINIMIZE THIS NUMBER (DISPENSING DURATION), based on max flow rate of pump
const unsigned long waterIntakeDuration = 30000; // Intake for 30 seconds (30000ms)

bool isWaterIntaking = false;
unsigned long intakeStartTime = 0;

// Start water intake
void startWaterIntake() {

    unsigned long currentTime = millis();

    float WATER_PUMP_PWM = WATER_PUMP_DUTY_CYCLE * 4950;

    // Start water intake if not already running
    if (!isWaterIntaking) {
        Serial.println("Starting water intake...");
        pwm.setPWM(PWM_WATER_PUMP_CHANNEL, 0, WATER_PUMP_PWM);

        isWaterIntaking = true;
        intakeStartTime = currentTime;
    }

    // Stop water intake after waterIntakeDuration seconds
    if (isWaterIntaking && (currentTime - intakeStartTime >= waterIntakeDuration)) {
        stopWaterIntake();
        isWaterIntaking = false;
    }

}

// Stop water intake
void stopWaterIntake() {
    Serial.println("Stopping water intake.");
    pwm.setPWM(PWM_WATER_PUMP_CHANNEL, 0, 0);
    resetWaterCmd();
}