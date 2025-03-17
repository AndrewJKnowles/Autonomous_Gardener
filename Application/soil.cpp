#include "soil.h"

#include <Arduino.h>
#include "debug_defs.h"

Soil::Soil(){};

/***/
void Soil::Get_Soil_Health(soil_health_t *soil_record)
{
  //save last adc reading
  soil_record->last_moisture_level_adc_raw = soil_record->moisture_level_adc_raw;

  //take new adc reading
  soil_record->moisture_level_adc_raw = analogRead(soil_record->soil_moisture_pin);

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