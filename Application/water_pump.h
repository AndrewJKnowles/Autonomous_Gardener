/************************************************************************
* @file     Autonomus_Gardener/water_pump.h
* @author   AndrewJKnowles
* @date     26/03/2025
* @version  1.0
* @brief    Available under the GNU GENERAL PUBLIC LICENSE without any 
*           warranty or liability
**************************************************************************/

#ifndef WATER_PUMP_H
#define WATER_PUMP_H

#include <stdint.h>

typedef enum
{
  PUMP_OFF  = 0x00,
  PUMP_ON   = 0x01,
}pump_states_t;

typedef struct
{
  pump_states_t state;
  uint8_t       enable_pin;
}water_pump_t;

class Pump
{
  public:
    Pump(water_pump_t *pump);
    void Set_Pump_State(water_pump_t *pump, pump_states_t state);
    void Action_Watering(water_pump_t *pump);
    void Test(water_pump_t *pump);
};

#endif //WATER_PUMP_H