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

class Dma {
 public:
  enum class Channel : uint8_t {};

  static Channel claim();
  static DmacDescriptor& descriptor(Channel channel);
  static DmacDescriptor& write_back_descriptor(Channel channel);
  static void enable(Channel channel, uint8_t trigger_action, uint8_t trigger_source, uint8_t priority);
  static void disable(Channel channel);
  static void trigger(Channel channel);
  static void enable_interrupt(Channel channel);
  static void disable_interrupt(Channel channel);

  template <typename Context>
  static void install_interrupt_handler(Channel channel, Context* context, void (*callback)(Context*))
  {
    _install_interrupt_handler(channel, context, reinterpret_cast<void (*)(void*)>(callback));
  }

 private:
  static void _install_interrupt_handler(Channel channel, void* context, void (*callback)(void*));
};

#endif
