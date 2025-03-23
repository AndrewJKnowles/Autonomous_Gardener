#include <Wire.h>

#include "water_pump.h"
#include "soil.h"
#include "pinout.h"
#include "debug_defs.h"
#include "pwr_mngr.h"
 
#define INTERRUPT_0           0             //pin D2
#define RTC_DEVICE_ID         0x68
#define SOIL_CHECK_INTERVAL   10             //s

/*************************************
* @brief Private Function Proto-types
*************************************/
static void RTC_Set_Int_Freq(void);
static void Interrupt_ISR(void);


Soil Soil;
PWR_MNGR PWR_MNGR;
soil_health_t soil_health = {0};
volatile uint32_t seconds_passed = 0;
bool check_soil = false;

//set up function
void setup()
{
  Serial.begin(9600);
  while (!Serial) ; // wait for serial

  soil_health.soil_moisture_pin = SOIL_MOISTURE_PIN;
  pinMode(LED_1,OUTPUT);

  attachInterrupt(INTERRUPT_0,Interrupt_ISR,FALLING);
  RTC_Set_Int_Freq();
}
 
//main loop
void loop()
{
  if(check_soil)
  {
    Soil.Get_Soil_Health(&soil_health);
    check_soil = false;

    if(soil_health.soil_state == SATURATED)
    {
      //replace with motor on when soil_state == DRY
      digitalWrite(LED_1, digitalRead(LED_1) ^ 1);
    }
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