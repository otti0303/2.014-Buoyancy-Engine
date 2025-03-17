#ifndef FUEL_CONTROL_H
#define FUEL_CONTROL_H

// Fuel Control
#define PWM_FUEL_CHANNEL 0
// #define FUEL_DUTY_CYCLE 725 // 72.5% duty cycle (mapped to 12-bit PWM: 0-4095) ??
#define MOSFET_FUEL  5  // Fuel pump MOSFET ??

// Ammonia Control
#define PWM_AMM_CHANNEL 1
// #define AMM_DUTY_CYCLE 105 // 10.5% duty cycle (mapped to 12-bit PWM: 0-4095) ??
#define MOSFET_AMM  5  // Ammonia pump MOSFET ??

void startFuelAmmDispensing();
// void stopAmmDispensing();
// void stopFuelDispensing();
void ABORT_fuel_amm();

#endif