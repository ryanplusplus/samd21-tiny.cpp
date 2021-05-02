/*!
 * @file
 * @brief
 */

#include "systick.h"
#include "samd21.h"

static struct {
  i_tiny_time_source_t interface;
  volatile tiny_time_source_ticks_t ticks;
} self;

static tiny_time_source_ticks_t skip_ticks;

void SysTick_Handler(void)
{
  if(skip_ticks) {
    self.ticks += skip_ticks - 1;
    skip_ticks = 0;

    SysTick_Config(48000000UL / 1000);
  }

  self.ticks++;
}

static tiny_time_source_ticks_t ticks(i_tiny_time_source_t* _self)
{
  (void)_self;

  tiny_time_source_ticks_t ticks1;
  tiny_time_source_ticks_t ticks2;

  do {
    ticks1 = self.ticks;
    ticks2 = self.ticks;
  } while(ticks1 != ticks2);

  return ticks1;
}

static const i_tiny_time_source_api_t api = { ticks };

i_tiny_time_source_t* systick_init(void)
{
  // fixme SystemCoreClock doesn't exist
  if(SysTick_Config(48000000UL / 1000)) {
    NVIC_SystemReset();
  }

  NVIC_SetPriority(SysTick_IRQn, 7);

  self.interface.api = &api;

  return &self.interface;
}

void systick_skip(tiny_time_source_ticks_t ticks)
{
  if(skip_ticks) {
    return;
  }

  tiny_time_source_ticks_t max_ticks_to_skip = SysTick_LOAD_RELOAD_Msk / (48000000UL / 1000);

  if(ticks > max_ticks_to_skip) {
    ticks = max_ticks_to_skip;
  }

  SysTick_Config((48000000UL / 1000) * ticks);

  skip_ticks = ticks;
}
