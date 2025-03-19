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

int timer1_compare_match;

ISR(TIMER1_COMPA_vect)
{
  digitalWrite(10, !digitalRead(10));
}

/***/
void setup()
{
  //SOIL_HEALTH.soil_moisture_pin = SOIL_MOISTURE_PIN;
  //Soil.LP_Filter_Settle(&SOIL_HEALTH);

  pinMode(10, OUTPUT);

  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  timer1_compare_match = 31249;
  TCNT1 = timer1_compare_match;
  TCCR1B |= (1 << CS12);
  TIMSK1 |= (1 << OCIE1A);
  interrupts();
}

/***/
void loop()
{

/*
  Soil.Get_Soil_Health(&SOIL_HEALTH);

  if(SOIL_HEALTH.soil_state == DRY)
  {
    Pump.Action_Watering(&WATER_PUMP);
  }

  delay(10000);
*/
}
