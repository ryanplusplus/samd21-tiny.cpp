/*!
 * @file
 * @brief
 */

#ifndef Neopixel_hpp
#define Neopixel_hpp

#include <cstdint>
#include "Interrupts.hpp"
#include "sam.h"

namespace Neopixel {
  enum class ByteOrder {
    grb,
    rgb,
    rgbw
  };

  template <ByteOrder byte_order>
  struct Color;

  template <>
  struct Color<ByteOrder::grb> {
    uint8_t g;
    uint8_t r;
    uint8_t b;
  };

  template <>
  struct Color<ByteOrder::rgb> {
    uint8_t r;
    uint8_t g;
    uint8_t b;
  };

  template <>
  struct Color<ByteOrder::rgbw> {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t w;
  };

  template <ByteOrder byte_order, uint32_t frequency, unsigned pin>
  class Neopixel {
    static_assert(frequency == 48000000);

   public:
    using Color = ::Neopixel::Color<byte_order>;

   public:
    Neopixel()
    {
      PORT->Group[pin / 32].PINCFG[pin % 32].bit.PMUXEN = 0;
      PORT->Group[pin / 32].DIRSET.reg = 1 << (pin % 32);
    }

   public:
    void write(const Color* data, uint16_t count)
    {
      Interrupts::critical_section([&]() {
        for(uint16_t i = 0; i < count; i++) {
          for(uint8_t j = 0; j < sizeof(data[0]); j++) {
            send_byte(((const uint8_t*)data)[i * sizeof(data[0]) + j]);
          }
        }
      });
    }

    void write_all(const Color* data, uint16_t count)
    {
      Interrupts::critical_section([&]() {
        for(uint16_t i = 0; i < count; i++) {
          for(uint8_t j = 0; j < sizeof(data[0]); j++) {
            send_byte(((const uint8_t*)data)[j]);
          }
        }
      });
    }

    void reset()
    {
      if constexpr(frequency == 48000000) {
        // 50 usec == 2400 cycles @ 48 MHz
        __asm__ __volatile__(
          "reset_neopixel:\n"
          // 0x350 == ~850
          "	 mov r4, #0x35\n"
          "	 lsl r4, #4\n"
          "loop:\n"
          "	 sub r4, r4, #1\n"
          "	 bne loop\n"
          :
          :
          : "r4");
      }
    }

   private:
    static void send_byte(uint8_t byte)
    {
      if constexpr(frequency == 48000000) {
        __asm__ __volatile__(
          "send_byte:\n"
          "  mov r4, #8\n"
          "\n"
          "send_bit:\n"
          "  str %[_mask], [%[_outset]]\n"
          "  nop\n"
          "  nop\n"
          "  nop\n"
          "  nop\n"
          "  nop\n"
          "  nop\n"
          "  nop\n"
          "  nop\n"
          "  nop\n"
          "  nop\n"
          "  lsl %[byte], %[byte], #1\n"
          "  tst %[byte], %[_carry]\n"
          "  bne send_1\n"
          "\n"
          "send_0:\n"
          "  str %[_mask], [%[_outclr]]\n"
          "\n"
          "	 mov r5, #8\n"
          "send_0_loop:\n"
          "	 sub r5, r5, #1\n"
          "  nop\n"
          "	 bne send_0_loop\n"
          "\n"
          "  nop\n"
          "  nop\n"
          "  sub r4, r4, #1\n"
          "  bne send_bit\n"
          "  b done\n"
          "\n"
          "send_1:\n"
          "  nop\n"
          "  nop\n"
          "  nop\n"
          "  nop\n"
          "  nop\n"
          "  nop\n"
          "  nop\n"
          "  nop\n"
          "  nop\n"
          "  nop\n"
          "  nop\n"
          "  nop\n"
          "  nop\n"
          "  sub r4, r4, #1\n"
          "  str %[_mask], [%[_outclr]]\n"
          "  nop\n"
          "  nop\n"
          "  nop\n"
          "  nop\n"
          "  nop\n"
          "  nop\n"
          "  nop\n"
          "  nop\n"
          "  nop\n"
          "  nop\n"
          "  nop\n"
          "  nop\n"
          "  nop\n"
          "  bne send_bit\n"
          "\n"
          "done:\n"
          :
          : [byte] "r"(byte), [_outset] "l"(&PORT->Group[pin / 32].OUTSET.reg), [_outclr] "l"(&PORT->Group[pin / 32].OUTCLR.reg), [_mask] "r"(1 << (pin % 32)), [_carry] "r"(0x100)
          : "r4", "r5", "memory");
      }
    }
  };
}

#endif
