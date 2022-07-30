/*!
 * @file
 * @brief
 */

#ifndef Interrupts_hpp
#define Interrupts_hpp

extern "C" {
#include "sam.h"
}

#include <cstdint>

class Interrupts {
 public:
  static void enable()
  {
    __enable_irq();
  }

  static void disable()
  {
    __disable_irq();
  }

  static void wait_for_interrupt()
  {
    __WFI();
  }

  static uint32_t save()
  {
    uint32_t primask = __get_PRIMASK();
    disable();
    return primask;
  }

  static void restore(uint32_t primask)
  {
    __set_PRIMASK(primask);
  }

  template <typename t>
  static void critical_section(t body)
  {
    auto state = Interrupts::save();
    body();
    Interrupts::restore(state);
  }
};

#endif
