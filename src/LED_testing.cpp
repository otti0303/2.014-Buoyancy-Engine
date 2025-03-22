// keyboard esp32 test
// upload this script to the esp32 using arduino ide, then quit arduino (such that port is not open in ide)
// then run laptop_cmd_test.py on laptop terminal which will connect to esp32 port using python
// will let you send commands with laptop keys

// UPLOAD TO ESP32 USING ARDUINO IDE

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
            } 
            
            else if (command == "DESCEND") {
                digitalWrite(LED17_PIN, HIGH);
                ventStartTime = millis();
                ventInProgress = true;
                last_comm = "DESCEND";
            } 
            
            else if (command == "ABORT_ASCENT") {
                digitalWrite(LED16_PIN, LOW);
                ascendInProgress = false;
                last_comm = "ABORT_ASCENT";
            } 
            
            else if (command == "ABORT_DESCENT") {
                digitalWrite(LED17_PIN, LOW);
                digitalWrite(LED18_PIN, LOW);
                ventInProgress = false;
                intakeInProgress = false;
                last_comm = "ABORT_DESCENT";
            } 
            
            else if (command == "ALL_OFF") {
                digitalWrite(LED16_PIN, LOW);
                digitalWrite(LED17_PIN, LOW);
                digitalWrite(LED18_PIN, LOW);
                digitalWrite(LED19_PIN, LOW);
                last_comm = "ALL_OFF";
            } 
            
            else if (command == "TURN_ON") {
                flashLEDs();
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