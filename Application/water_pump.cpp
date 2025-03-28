/************************************************************************
* @file     Autonomus_Gardener/water_pump.cpp
* @author   AndrewJKnowles
* @date     26/03/2025
* @version  1.0
* @brief    Available under the GNU GENERAL PUBLIC LICENSE without any 
*           warranty or liability
**************************************************************************/

#include "water_pump.h"

#include <Arduino.h>
#include "debug_defs.h"

/***/
Pump::Pump(water_pump_t *pump)
{
  pinMode(pump->enable_pin, OUTPUT);
  Set_Pump_State(pump, pump->state);
}

/***/
void Pump::Set_Pump_State(water_pump_t *pump, pump_states_t state)
{
  digitalWrite(pump->enable_pin, (uint8_t)state);
  pump->state = state;
}

/***/
void Pump::Action_Watering(water_pump_t *pump)
{
#ifdef ENABLE_SERIAL_PRINTS
  Serial.print("Watering Plant\n");
#endif  //ENABLE_SERIAL_PRINTS

  Set_Pump_State(pump, PUMP_ON);
  delay(10000);
  Set_Pump_State(pump, PUMP_OFF);
}

void Pump::Test(water_pump_t *pump)
{
  digitalWrite(pump->enable_pin, 1);
  delay(1000);
  digitalWrite(pump->enable_pin, 0);
  delay(1000);
}