/*!
 * @file
 * @brief
 */

#ifndef Dma_hpp
#define Dma_hpp

extern "C" {
#include "sam.h"
}

#include <cstdint>

namespace Dma {
  using InterruptHandler = void (*)();
  enum class Channel : uint8_t {};

  Channel claim();
  DmacDescriptor& descriptor(Channel channel);
  DmacDescriptor& write_back_descriptor(Channel channel);
  void enable(Channel channel, uint8_t trigger_action, uint8_t trigger_source, uint8_t priority);
  void disable(Channel channel);
  void trigger(Channel channel);
  void enable_interrupt(Channel channel);
  void disable_interrupt(Channel channel);
  void install_interrupt_handler(Channel channel, InterruptHandler handler);
}

#endif
