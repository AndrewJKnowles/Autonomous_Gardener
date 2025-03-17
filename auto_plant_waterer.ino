#include <DFRobot_DHT11.h>
#include "avr/sleep.h"
#include "avr/interrupt.h"

/*********************
* @brief Debug Defines
**********************/
//#define ENABLE_SERIAL_PRINTS

/************************
* @brief Generic Defines
*************************/
#define SOIL_MOISTURE_M_COEF      (float)0.454545
#define SOIL_MOISTURE_C_COEF      (float)-227.272724
#define SOIL_STATE_UPPER_THRESH   60 //%
#define SOIL_STATE_LOWER_THRESH   30 //%

/********************************
* @brief Digital Pin Definitions
*********************************/
#define D10 10
#define D6  6
#define D5  5

/********************************
* @brief Analogue Pin Definitions
*********************************/
#define SOIL_MOISTURE_PIN A2  

/************************
* @brief Enums & Structs
************************/
typedef enum
{
  PUMP_OFF  = 0x00,
  PUMP_ON   = 0x01,
}pump_states_t;

typedef struct
{
  pump_states_t state;
  uint8_t       enable_pin1;
  uint8_t       enable_pin2;
}water_pump_t;

typedef enum
{
  SATURATED,
  OK,
  DRY
}soil_state_t;

typedef struct
{
  soil_state_t soil_state;
  float        moisture_level_percentage;

  int          moisture_level_adc_raw;
  int          last_moisture_level_adc_raw;
}soil_health_t;

/************************************
* @brief Private Function Proto-types
*************************************/
static void Init_Pump(water_pump_t *pump);
static void Set_Pump_State(water_pump_t *pump, pump_states_t state);
static void Action_Watering(water_pump_t *pump);
static void Get_Soil_Health(soil_health_t *soil_record);

/************************
* @brief Public Variables
*************************/
water_pump_t WATER_PUMP =
{
  .state       = PUMP_OFF,
  .enable_pin1 = D6,
  .enable_pin2 = D5,
};

soil_health_t SOIL_HEALTH = {0};



/***/
void setup()
{
#ifdef ENABLE_SERIAL_PRINTS
  Serial.print("Set up Started...\n");
#endif //ENABLE_SERIAL_PRINTS

  Init_Pump(&WATER_PUMP);

  //allow Get_Soil_Health filter to stabilise
  for(int i = 0; i < 10; i++)
  {
    Get_Soil_Health(&SOIL_HEALTH);
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

  sleep_mode();
/*
  Get_Soil_Health(&SOIL_HEALTH);

  if(SOIL_HEALTH.soil_state == DRY)
  {
    Action_Watering(&WATER_PUMP);
  }

  delay(10000);
*/
}



/*************************
* @brief Private Functions
**************************/

/***/
static void Get_Soil_Health(soil_health_t *soil_record)
{
  //save last adc reading
  soil_record->last_moisture_level_adc_raw = soil_record->moisture_level_adc_raw;

  //take new adc reading
  soil_record->moisture_level_adc_raw = analogRead(SOIL_MOISTURE_PIN);

  //pass new adc reading through basic low-pass filter
  soil_record->moisture_level_adc_raw = (soil_record->moisture_level_adc_raw + soil_record->last_moisture_level_adc_raw)/2;

  //apply conversion boundaries
  if(soil_record->moisture_level_adc_raw < 0)
  {
    soil_record->moisture_level_adc_raw = 0;
  }
  else if(soil_record->moisture_level_adc_raw > 720)
  {
    soil_record->moisture_level_adc_raw = 720;
  }

  //convert adc to percentage
  soil_record->moisture_level_percentage = (float)((soil_record->moisture_level_adc_raw * SOIL_MOISTURE_M_COEF) + SOIL_MOISTURE_C_COEF);

  //set soil health state
  if((int)soil_record->moisture_level_percentage >= SOIL_STATE_UPPER_THRESH)
  {
    soil_record->soil_state = SATURATED;
  }
  else if((int)soil_record->moisture_level_percentage <= SOIL_STATE_LOWER_THRESH)
  {
    soil_record->soil_state = DRY;
  }
  else
  {
    soil_record->soil_state = OK;
  }

#ifdef ENABLE_SERIAL_PRINTS
  Serial.print("Soil Moisture: ");
  Serial.print(soil_record->moisture_level_percentage);
  Serial.print("% Raw Reading: ");
  Serial.print(soil_record->moisture_level_adc_raw);
  Serial.print(" Soil State: ");
  Serial.print(soil_record->soil_state);
  Serial.print("\n");
#endif //ENABLE_SERIAL_PRINTS
}

/***/
static void Init_Pump(water_pump_t *pump)
{
  pinMode(pump->enable_pin1, OUTPUT);
  pinMode(pump->enable_pin2, OUTPUT);

  Set_Pump_State(pump, pump->state);
}

/***/
static void Set_Pump_State(water_pump_t *pump, pump_states_t state)
{
  digitalWrite(pump->enable_pin1, (uint8_t)state);
  digitalWrite(pump->enable_pin2, (uint8_t)state);
  pump->state = state;
}

/***/
static void Action_Watering(water_pump_t *pump)
{
  Set_Pump_State(pump, PUMP_ON);
  delay(1000);
  Set_Pump_State(pump, PUMP_OFF);
}
