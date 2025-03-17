#ifndef VENT_CONTROL_H
#define VENT_CONTROL_H

// Vent Control
#define PWM_VENT_CHANNEL 2
#define VENT_MAX_DUTY 2000
#define VENT_SETPOINT 1.0
#define MOSFET_VENT  6  // Venting pump MOSFET ??

void startGasVent();
void stopGasVent();
void ABORT_vent_and_intake();

#endif