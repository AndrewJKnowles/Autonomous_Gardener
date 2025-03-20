#include "pwr_mngr.h"
#include <avr/sleep.h>

PWR_MNGR::PWR_MNGR(void){}

void PWR_MNGR::Enter_Sleep(void)
{
  sleep_enable();
  set_sleep_mode(SLEEP_MODE_STANDBY);
  sleep_mode();
}