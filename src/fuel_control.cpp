#include "fuel_control.h"
#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include "config.h"
#include "vent_control.h"
#include "water_intake_control.h"

extern Adafruit_PWMServoDriver pwm;

// use / define MOSFET to drive which pumps??

// UPDATE THESE SET VALUES
#define NEED_AMM 19.58 // mL
#define NEED_FUEL 7.1 // mL

#define VOLTAGE_AMM_PUMP 12 // V // to be changed based on pump (prob 6 V)
#define VOLTAGE_FUEL_PUMP 12 // V // to be changed based on pump (prob 6 V)

#define MAX_FLOW_RATE_AMM_PUMP 18 // mL/min // flow rate to be changed based on pump choice
#define MAX_FLOW_RATE_FUEL_PUMP 45 // mL/min // flow rate to be changed based on pump choice

// WANT TO MINIMIZE THESE NUMBERS (DISPENSING DURATION), based on max flow rate of the pumps
const unsigned long ammoniaPumpDuration = 30000; // Pump for 30 seconds (30000ms)
const unsigned long fuelPumpDuration = 20000; // Pump for 20 seconds (20000ms)

bool isAmmoniaPumping = false;
bool isFuelPumping = false;
unsigned long pumpStartTime = 0;

float getAmmDutyCycle() {
    float K_amm = MAX_FLOW_RATE_AMM_PUMP / VOLTAGE_AMM_PUMP / 60;
    float Q_amm = NEED_AMM / ammoniaPumpDuration;
    float U_amm = Q_amm / K_amm; // voltage to send across amm pump

    float PWM_amm = U_amm / VOLTAGE_AMM_PUMP * 4095; // divided by 100?

    return PWM_amm;
}

float getFuelDutyCycle() {
    float K_fuel = MAX_FLOW_RATE_FUEL_PUMP / VOLTAGE_FUEL_PUMP / 60;
    float Q_fuel = NEED_FUEL / fuelPumpDuration;
    float U_fuel = Q_fuel / K_fuel; // voltage to send across fuel pump

    float PWM_fuel = U_fuel / VOLTAGE_FUEL_PUMP * 4095; // divided by 100?

    return PWM_fuel;
}

// Start fuel dispensing
void startFuelAmmDispensing() {

    unsigned long currentTime = millis();

    float AMM_DUTY_C = getAmmDutyCycle();
    float FUEL_DUTY_C = getFuelDutyCycle();

    // fuel and ammonia get pumped at the same time

    // Start pumps if not already running
    if (!isAmmoniaPumping && !isFuelPumping) {
        Serial.println("Starting ammonia dispensing...");
        pwm.setPWM(PWM_AMM_CHANNEL, 0, AMM_DUTY_C);

        Serial.println("Starting fuel dispensing...");
        pwm.setPWM(PWM_FUEL_CHANNEL, 0, FUEL_DUTY_C);

        isAmmoniaPumping = true;
        isFuelPumping = true;
        pumpStartTime = currentTime;
    }

    // Stop ammonia pump after ammoniaPumpDuration seconds
    if (isAmmoniaPumping && (currentTime - pumpStartTime >= ammoniaPumpDuration)) {
        stopAmmDispensing();
        isAmmoniaPumping = false;
    }

    // Stop fuel pump after fuelPumpDuration seconds
    if (isFuelPumping && (currentTime - pumpStartTime >= fuelPumpDuration)) {
        stopFuelDispensing();
        isFuelPumping = false;
    }
}

// Stop ammonia dispensing
void stopAmmDispensing() {
    Serial.println("Stopping ammonia dispensing.");
    pwm.setPWM(PWM_AMM_CHANNEL, 0, 0);
    resetFuelCmd();
}

// Stop fuel dispensing
void stopFuelDispensing() {
    Serial.println("Stopping fuel dispensing.");
    pwm.setPWM(PWM_FUEL_CHANNEL, 0, 0);
    resetFuelCmd();
}

void ABORT_fuel_amm() {
    // stop power to pumps
    stopFuelDispensing();
    stopAmmDispensing();

    // vent bladder
    startGasVent(); // automatically starts water intake when gas vent stops
}