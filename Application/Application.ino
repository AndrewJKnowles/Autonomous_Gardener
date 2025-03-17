#include "water_pump.h"
#include "soil.h"
#include "pinout.h"
#include "debug_defs.h"
#include "pwr_mngr.h"

water_pump_t WATER_PUMP =
{
  .state       = PUMP_OFF,
  .enable_pin1 = D6,
  .enable_pin2 = D5,
};

soil_health_t SOIL_HEALTH = {0};

Pump Pump(&WATER_PUMP);
Soil Soil;
PWR_MNGR PWR;

/***/
void setup()
{
  SOIL_HEALTH.soil_moisture_pin = SOIL_MOISTURE_PIN;
  Soil.LP_Filter_Settle(&SOIL_HEALTH);
}

/***/
void loop()
{
  Soil.Get_Soil_Health(&SOIL_HEALTH);

  if(SOIL_HEALTH.soil_state == DRY)
  {
    Pump.Action_Watering(&WATER_PUMP);
  }

  delay(10000);
}
