/*!
 * @file
 * @brief
 */

extern "C" {
#include "sam.h"
}

#include "Clock.hpp"
#include "SystemTick.hpp"

using namespace tiny;

static volatile ITimeSource::TickCount current_ticks;

extern "C" void SysTick_Handler()
{
  current_ticks = current_ticks + 1;
}

static class : public ITimeSource {
 public:
  void init()
  {
    if(SysTick_Config(Clock::gclk_main_frequency / 1000)) {
      NVIC_SystemReset();
    }

    NVIC_SetPriority(SysTick_IRQn, 7);
  }

  TickCount ticks() const override
  {
    TickCount previous;
    TickCount current = current_ticks;

    do {
      previous = current;
      current = current_ticks;
    } while(current != previous);

    return current;
  }
} instance;

ITimeSource& SystemTick::get_instance()
{
  instance.init();
  return instance;
}
