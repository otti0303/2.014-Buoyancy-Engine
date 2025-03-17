#include "vent_control.h"
#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include "water_intake_control.h"
#include "config.h"

extern Adafruit_PWMServoDriver pwm;

// use / define MOSFET to drive this pump??

// GAS VENT OCCURS FOR A SET AMOUNT OF TIME EACH TIME (until bladders are empty)

// set a certain appropriate duty cycle ... ask chem team for numbers
// calculate duty cycle based on pumps (flow rate / voltages)
#define GAS_VENT_PUMP_DUTY_CYCLE = 0.5

// WANT TO MINIMIZE THIS NUMBER (DISPENSING DURATION), based on max flow rate of pump
const unsigned long gasVentDuration = 20000; // Vent for 20 seconds (20000ms)

bool isGasVenting = false;
unsigned long ventStartTime = 0;

// Start gas vent
void startGasVent() {

    unsigned long currentTime = millis();

    float GAS_VENT_PUMP_PWM = GAS_VENT_PUMP_DUTY_CYCLE * 4950;

    // Start gas venting if not already running
    if (!isGasVenting) {
        Serial.println("Starting gas vent...");
        pwm.setPWM(PWM_VENT_CHANNEL, 0, GAS_VENT_PUMP_PWM);
        isGasVenting = true;
        ventStartTime = currentTime;
    }

    // Stop gas venting after gasVentDuration seconds
    if (isGasVenting && (currentTime - ventStartTime >= gasVentDuration)) {
        stopGasVent(); // also starts the water intake once gas vent stops
        isGasVenting = false;
    }

}

// Stop gas vent
void stopGasVent() {
    Serial.println("Stopping gas vent.");
    pwm.setPWM(PWM_VENT_CHANNEL, 0, 0);
    startWaterIntake();
}

void ABORT_vent_and_intake() {
    // stop power to gas vent pump
    Serial.println("Stopping gas vent.");
    pwm.setPWM(PWM_VENT_CHANNEL, 0, 0);

    // stop power to water intake pump
    Serial.println("Stopping water intake.");
    pwm.setPWM(PWM_WATER_PUMP_CHANNEL, 0, 0);
}