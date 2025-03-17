#include <avr/sleep.h>

#include "water_pump.h"
#include "soil.h"
#include "pinout.h"
#include "debug_defs.h"

/************************
* @brief Enums & Structs
************************/
water_pump_t WATER_PUMP =
{
  .state       = PUMP_OFF,
  .enable_pin1 = D6,
  .enable_pin2 = D5,
};

soil_health_t SOIL_HEALTH =
{
  .soil_moisture_pin            = SOIL_MOISTURE_PIN,
  .soil_state                   = 0,
  .moisture_level_percentage    = 0,
  .moisture_level_adc_raw       = 0,
  .last_moisture_level_adc_raw  = 0,
};

Pump Pump(&WATER_PUMP);
Soil Soil;

/***/
void setup()
{
#ifdef ENABLE_SERIAL_PRINTS
  Serial.print("Set up Started...\n");
#endif //ENABLE_SERIAL_PRINTS

  //allow Get_Soil_Health filter to stabilise
  for(int i = 0; i < 10; i++)
  {
    Soil.Get_Soil_Health(&SOIL_HEALTH);
    delay(5000);
  }

  sleep_enable();
  set_sleep_mode(SLEEP_MODE_PWR_SAVE);

#ifdef ENABLE_SERIAL_PRINTS
  Serial.print("Set up complete\n");
#endif //ENABLE_SERIAL_PRINTS
}

/***/
void loop()
{

  //sleep_mode();

  Soil.Get_Soil_Health(&SOIL_HEALTH);

  if(SOIL_HEALTH.soil_state == DRY)
  {
    Pump.Action_Watering(&WATER_PUMP);
  }

  delay(10000);
}
