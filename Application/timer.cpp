#include "timer.h"

#include "Arduino.h"

Timer::Timer(uint16_t crv)
{
  if(crv == 0)
  {
    timer1_compare_match = 62500; //default to 1 Hz
  }
  else
  {
    timer1_compare_match = crv;
  }
}

void Timer::Init(bool en)
{
  noInterrupts(); //Disable Interrupts
  TCCR1A = 0; 
  TCCR1B = 0;
  TCNT1 = timer1_compare_match; //load crv
  TCCR1B |= (1 << CS12);        //set prescalar to 256

  if(en)
  {
    Enable_Int();
  }

  interrupts();
}

void Timer::Enable_Int(void)
{
  noInterrupts(); //Disable Interrupts
  TIMSK1 |= 0x02;     //enable timer1 interrupt
  interrupts();
}

void Timer::Disable_Int(void)
{
  noInterrupts(); //Disable Interrupts
  TIMSK1 &= 0xFD;     //enable timer1 interrupt
  interrupts();
}