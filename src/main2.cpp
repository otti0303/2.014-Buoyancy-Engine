// WITH LEDS
// originally LED testing script, with added functionalities (commented out) for pump control
// to help with testing

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

#define LED16_PIN 16  // REP AMMONIA PUMP
#define LED17_PIN 17 // REP GAS VENT PUMP
#define LED18_PIN 18 // REP WATER INTAKE PUMP
#define LED19_PIN 19 // REP ALUMINUM PASTE PUMP

void setup() {
    pinMode(LED16_PIN, OUTPUT);
    pinMode(LED17_PIN, OUTPUT);
    pinMode(LED18_PIN, OUTPUT);
    pinMode(LED19_PIN, OUTPUT);
    Serial.begin(115200);

    pwm.begin();
    pwm.setPWMFreq(PWM_FREQUENCY);

    Serial.println("ESP32-S3 Initialized!");
}

void flashLEDs() {
    for (int i = 0; i < 5; i++) { 
        digitalWrite(LED16_PIN, HIGH);
        digitalWrite(LED17_PIN, HIGH);
        digitalWrite(LED18_PIN, HIGH);
        digitalWrite(LED19_PIN, HIGH);
        delay(100);
        digitalWrite(LED16_PIN, LOW); 
        digitalWrite(LED17_PIN, LOW); 
        digitalWrite(LED18_PIN, LOW); 
        digitalWrite(LED19_PIN, LOW); 
        delay(100); 
    }
}

unsigned long ascendStartTime = 0;
const unsigned long ascendDuration = 3000;

unsigned long ventStartTime = 0;
const unsigned long ventDuration = 3000;

unsigned long intakeStartTime = 0;
const unsigned long intakeDuration = 3000;

bool ascendInProgress = false;
bool ventInProgress = false;
bool intakeInProgress = false;

String last_comm = "";

void loop() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');  // Read the incoming command
        command.trim();

        if (last_comm != command) {

            if (command == "ASCEND") {
                digitalWrite(LED16_PIN, HIGH); 
                digitalWrite(LED19_PIN, HIGH);
                ascendStartTime = millis();  // Record the time when ASCEND command was received
                ascendInProgress = true;
                last_comm = "ASCEND";

                // NON TEST CODE
                // startFuelAmmDispensing();
            } 
            
            else if (command == "DESCEND") {
                digitalWrite(LED17_PIN, HIGH);
                ventStartTime = millis();
                ventInProgress = true;
                last_comm = "DESCEND";

                // NON TEST CODE
                // startGasVent();
            } 
            
            else if (command == "ABORT_ASCENT") {
                digitalWrite(LED16_PIN, LOW);
                ascendInProgress = false;
                last_comm = "ABORT_ASCENT";

                // NON TEST CODE
                // ABORT_fuel_amm();
            } 
            
            else if (command == "ABORT_DESCENT") {
                digitalWrite(LED17_PIN, LOW);
                digitalWrite(LED18_PIN, LOW);
                ventInProgress = false;
                intakeInProgress = false;
                last_comm = "ABORT_DESCENT";

                // NON TEST CODE
                // ABORT_vent_and_intake();
            } 
            
            else if (command == "ALL_OFF") {
                digitalWrite(LED16_PIN, LOW);
                digitalWrite(LED17_PIN, LOW);
                digitalWrite(LED18_PIN, LOW);
                digitalWrite(LED19_PIN, LOW);
                last_comm = "ALL_OFF";

                // NON TEST CODE
                // ???
                // ABORT_fuel_amm();
                // ABORT_vent_and_intake();
            } 
            
            else if (command == "TURN_ON") {
                flashLEDs();

                // NON TEST CODE
                // ???
            }
            
        }

    }

        if ((millis() - ascendStartTime >= ascendDuration) && ascendInProgress) {
            digitalWrite(LED16_PIN, LOW);
            digitalWrite(LED19_PIN, LOW);
            ascendInProgress = false;
        }

        if ((millis() - ventStartTime >= ventDuration) && ventInProgress) {
            digitalWrite(LED17_PIN, LOW);
            ventInProgress = false;
            digitalWrite(LED18_PIN, HIGH);
            intakeStartTime = millis();
            intakeInProgress = true;
        }

        if ((millis() - intakeStartTime >= intakeDuration) && intakeInProgress) {
            digitalWrite(LED18_PIN, LOW);
            intakeInProgress = false;
        }    
        
}