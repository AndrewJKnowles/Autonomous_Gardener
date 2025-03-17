#include "water_pump.h"

#include <Arduino.h>

/***/
Pump::Pump(water_pump_t *pump)
{
  pinMode(pump->enable_pin1, OUTPUT);
  pinMode(pump->enable_pin2, OUTPUT);

  Set_Pump_State(pump, pump->state);
}

/***/
void Pump::Set_Pump_State(water_pump_t *pump, pump_states_t state)
{
  digitalWrite(pump->enable_pin1, (uint8_t)state);
  digitalWrite(pump->enable_pin2, (uint8_t)state);
  pump->state = state;
}

/***/
void Pump::Action_Watering(water_pump_t *pump)
{
  Set_Pump_State(pump, PUMP_ON);
  delay(1000);
  Set_Pump_State(pump, PUMP_OFF);
}