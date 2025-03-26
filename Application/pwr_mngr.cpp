/************************************************************************
* @file     Autonomus_Gardener/pwr_mngr.cpp
* @author   AndrewJKnowles
* @date     26/03/2025
* @version  1.0
* @brief    Available under the GNU GENERAL PUBLIC LICENSE without any 
*           warranty or liability
**************************************************************************/

#include "pwr_mngr.h"
#include <avr/sleep.h>
#include <avr/power.h>

PWR_MNGR::PWR_MNGR(void){}

void PWR_MNGR::Enter_Sleep(void)
{
  sleep_enable();
  set_sleep_mode(SLEEP_MODE_STANDBY);
  sleep_mode();

  sleep_disable();
  power_all_enable();
}