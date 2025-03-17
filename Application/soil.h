#ifndef SOIL_H
#define SOIL_H

#include <stdint.h>

#define SOIL_MOISTURE_M_COEF      (float)0.454545
#define SOIL_MOISTURE_C_COEF      (float)-227.272724
#define SOIL_STATE_UPPER_THRESH   60 //%
#define SOIL_STATE_LOWER_THRESH   30 //%
#define SOIL_SETTLING_DELAY       5000 //mS
#define SOIL_SETTLING_REPETITIONS 10

typedef enum
{
  SATURATED,
  OK,
  DRY
}soil_state_t;

typedef struct
{
  uint8_t      soil_moisture_pin;
  soil_state_t soil_state;
  float        moisture_level_percentage;

  int          moisture_level_adc_raw;
  int          last_moisture_level_adc_raw;
}soil_health_t;

class Soil
{
  public:
    Soil(void);
    void LP_Filter_Settle(soil_health_t *soil_record);
    void Get_Soil_Health(soil_health_t *soil_record);
};

#endif //SOIL_H