#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>
#include <stdint.h>

class Timer
{
  public:
    Timer(uint16_t crv);
    void Init(bool);
    void Enable_Int(void);
    void Disable_Int(void);

  private:
    int timer1_compare_match;
};

#endif  //TIMER_H