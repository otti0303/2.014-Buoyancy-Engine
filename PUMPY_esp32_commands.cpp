// PUMPY ESP32 CPP SCRIPT FOR COMMANDS

// UPLOAD TO ESP32
// upload this script to the ESP32 using arduino IDE, then quit arduino (such that port is not open/being used by arduino IDE)
// then run user_GUI.py on laptop terminal which will connect to ESP32 port using python

#define ALUMINUM_PUMP 16 
#define AMMONIA_PUMP 17 
#define GAS_VENT_PUMP 18
#define WATER_INTAKE_PUMP 19

void setup() {
    pinMode(ALUMINUM_PUMP, OUTPUT);
    pinMode(AMMONIA_PUMP, OUTPUT);
    pinMode(GAS_VENT_PUMP, OUTPUT);
    pinMode(WATER_INTAKE_PUMP, OUTPUT);

    digitalWrite(AMMONIA_PUMP, LOW);
    digitalWrite(ALUMINUM_PUMP, LOW);
    digitalWrite(GAS_VENT_PUMP, LOW);
    digitalWrite(WATER_INTAKE_PUMP, LOW);

    Serial.begin(115200);
}

unsigned long ascendStartTime = 0;
const unsigned long ascendDuration = 3000;

const unsigned long ammoniaDuration = 3000;
const unsigned long aluminumDuration = 3000;

unsigned long ventStartTime = 0;
const unsigned long ventDuration = 3000;

unsigned long intakeStartTime = 0;
const unsigned long intakeDuration = 3000;

unsigned long ventHighStartTime = 0;
const unsigned long ventTimeHigh = 30;

unsigned long ventLowStartTime = 0;
const unsigned long ventTimeLow = 1;

unsigned long intakeHighStartTime = 0;
const unsigned long intakeTimeHigh = 30;

unsigned long intakeLowStartTime = 0;
const unsigned long intakeTimeLow = 1;

unsigned long ammoniaHighStartTime = 0;
const unsigned long ammoniaTimeHigh = 20;

unsigned long ammoniaLowStartTime = 0;
const unsigned long ammoniaTimeLow = 80;

unsigned long aluminumHighStartTime = 0;
const unsigned long aluminumTimeHigh = 30;

unsigned long aluminumLowStartTime = 0;
const unsigned long aluminumTimeLow = 1;

bool ascendInProgress = false;
bool ventInProgress = false;
bool intakeInProgress = false;

bool ammoniaHigh = false;
bool aluminumHigh = false;

bool ventHigh = false;
bool intakeHigh = false;

String last_comm = "";

void loop() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
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
                ventHighStartTime = millis();
                ventHigh = true;
                last_comm = "DESCEND";
            } 
            
            else if (command == "ABORT_ASCENT") {
                digitalWrite(AMMONIA_PUMP, LOW);
                digitalWrite(ALUMINUM_PUMP, LOW);

                ascendInProgress = false;
                last_comm = "ABORT_ASCENT";
                ammoniaHigh = false;
                aluminumHigh = false;

                Serial.println("TASK COMPLETED");
                last_comm = "RESET";
            } 
            
            else if (command == "ABORT_DESCENT") {
                digitalWrite(GAS_VENT_PUMP, LOW);
                digitalWrite(WATER_INTAKE_PUMP, LOW);

                ventInProgress = false;
                intakeInProgress = false;
                last_comm = "ABORT_DESCENT";
                ventHigh = false;
                intakeHigh = false;

                Serial.println("TASK COMPLETED");
                last_comm = "RESET";
            } 

            // don't need this command
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
                ammoniaHigh = false;
                
                if (ammoniaDuration >= aluminumDuration) {
                    ascendInProgress = false;
                    Serial.println("TASK COMPLETED");
                    last_comm = "RESET";
                }
            }

            if (millis() - ascendStartTime >= aluminumDuration) {
                digitalWrite(ALUMINUM_PUMP, LOW);
                aluminumHigh = false;
    
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

        if (ventInProgress) {

            if (millis() - ventStartTime >= ventDuration) {

                digitalWrite(GAS_VENT_PUMP, LOW);

                ventInProgress = false;
                ventHigh = false;

                digitalWrite(WATER_INTAKE_PUMP, HIGH);

                intakeStartTime = millis();
                intakeInProgress = true;
                intakeHigh = true;
            }

        }

        if (ventInProgress) {
            if (ventHigh && (millis() - ventHighStartTime >= ventTimeHigh)) {
                digitalWrite(GAS_VENT_PUMP, LOW);
                ventHigh = false;
            }

            if (!ventHigh && (millis() - ventLowStartTime >= ventTimeLow)) {
                digitalWrite(GAS_VENT_PUMP, HIGH);
                ventHigh = true;
            }
        }

        if (intakeInProgress) {

            if (millis() - intakeStartTime >= intakeDuration) {

                digitalWrite(WATER_INTAKE_PUMP, LOW);

                intakeInProgress = false;
                intakeHigh = false;
                Serial.println("TASK COMPLETED");
                last_comm = "RESET";

            }
        }

        if (intakeInProgress) {

            if (intakeHigh && (millis() - intakeHighStartTime >= intakeTimeHigh)) {
                digitalWrite(WATER_INTAKE_PUMP, LOW);
                intakeHigh = false;
            }

            if (!intakeHigh && (millis() - intakeLowStartTime >= intakeTimeLow)) {
                digitalWrite(WATER_INTAKE_PUMP, HIGH);
                intakeHigh = true;
            }

        }
}