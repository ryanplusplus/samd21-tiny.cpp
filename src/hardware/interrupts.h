/*!
 * @file
 * @brief
 */

#ifndef interrupts_h
#define interrupts_h

#include "sam.h"

static inline void interrupts_enable(void)
{
  __enable_irq();
}

static inline void interrupts_disable(void)
{
  __disable_irq();
}

static inline void interrupts_wait_for_interrupt(void)
{
  __WFI();
}

#endif
