#ifndef WATER_INTAKE_CONTROL_H
#define WATER_INTAKE_CONTROL_H

// Water Intake Control
#define PWM_WATER_PUMP_CHANNEL 2
#define MAX_FLOW_RATE 500 // mL/min, adjust based on pump specs
#define DEPTH_TOLERANCE 0.1 // Allowable depth error before stopping intake

void startWaterIntake();
void stopWaterIntake();

#endif