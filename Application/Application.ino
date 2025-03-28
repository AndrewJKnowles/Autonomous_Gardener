/************************************************************************
* @file     Autonomus_Gardener/Application.ino
* @author   AndrewJKnowles
* @date     26/03/2025
* @version  1.0
* @brief    Available under the GNU GENERAL PUBLIC LICENSE without any 
*           warranty or liability
**************************************************************************/

#include <Wire.h>

#include "water_pump.h"
#include "soil.h"
#include "pinout.h"
#include "debug_defs.h"
#include "pwr_mngr.h"
 
#define INTERRUPT_0           0             //pin D2
#define RTC_DEVICE_ID         0x68

#ifdef EN_DEBUGGING
#define SOIL_CHECK_INTERVAL   10             //s
#else
#define SOIL_CHECK_INTERVAL   60             //s
#endif //EN_DEBUGGING

/*************************************
* @brief Private Function Proto-types
*************************************/
static void RTC_Set_Int_Freq(void);
static void Interrupt_ISR(void);

/*************************
* @brief Global Variables
**************************/
soil_health_t soil_health = {0};
water_pump_t pump_cfg =
{
  .state      = PUMP_OFF,
  .enable_pin = RELAY_SWITCH
};

volatile uint32_t seconds_passed = 0;
volatile bool check_soil = true;

Soil Soil;
PWR_MNGR PWR_MNGR;
Pump  Pump(&pump_cfg);

//set up function
void setup()
{
#ifdef EN_DEBUGGING
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
#endif //EN_DEBUGGING

  pinMode(LED_1,OUTPUT);

  soil_health.soil_moisture_pin = SOIL_MOISTURE_PIN;
  Soil.LP_Filter_Settle(&soil_health);
  
  attachInterrupt(INTERRUPT_0,Interrupt_ISR,FALLING);
  RTC_Set_Int_Freq();

#ifdef EN_DEBUGGING
  Pump.Action_Watering(&pump_cfg);
#endif //EN_DEBUGGING
}
 
//main loop
void loop()
{
  if(check_soil)
  {
    digitalWrite(LED_1, 1);
    Soil.Get_Soil_Health(&soil_health);

    if(soil_health.soil_state == DRY)
    {
      Pump.Action_Watering(&pump_cfg);
    }
    
    digitalWrite(LED_1, 0);
    check_soil = false;
  }
  
  PWR_MNGR.Enter_Sleep();
}


/**************************
* @brief Private Functions
***************************/
static void RTC_Set_Int_Freq(void)
{
  Wire.begin();                           //create I2C tx queue   
  Wire.beginTransmission(RTC_DEVICE_ID);  //device address and STSRT command are queued
  Wire.write(0x0E);                       //Control Register Address is queued
  Wire.write(0x00);                       //Data for Control Register is queued
  Wire.endTransmission();                 //queued information are transferred under ACK; STOP
}

/***/
static void Interrupt_ISR(void)
{
  if(++seconds_passed == SOIL_CHECK_INTERVAL)
  {
      check_soil = true;
      seconds_passed = 0;
  }
}