/************************************************************************
* @file     Autonomus_Gardener/soil.h
* @author   AndrewJKnowles
* @date     26/03/2025
* @version  1.0
* @brief    Available under the GNU GENERAL PUBLIC LICENSE without any 
*           warranty or liability
**************************************************************************/

#ifndef SOIL_H
#define SOIL_H

#include <stdint.h>

#define SOIL_MOISTURE_M_COEF         (float)0.64516129
#define SOIL_MOISTURE_C_COEF          (float)-480.645161
#define SOIL_STATE_UPPER_THRESH       70 //%
#define SOIL_STATE_LOWER_THRESH       60 //%
#define SOIL_STATE_UPPER_ADC_THRESH   900
#define SOIL_STATE_LOWER_ADC_THRESH   745
#define SOIL_SETTLING_DELAY           1000 //mS
#define SOIL_SETTLING_REPETITIONS     30

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