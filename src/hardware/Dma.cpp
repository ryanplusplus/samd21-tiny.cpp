/*!
 * @file
 * @brief
 */

extern "C" {
#include "sam.h"
}

#include <cstdbool>
#include "Dma.hpp"
#include "Interrupts.hpp"

enum {
  channel_count = 12
};

static DmacDescriptor _descriptor[channel_count] __attribute__((__aligned__(16)));
static DmacDescriptor _write_back_descriptor[channel_count] __attribute__((__aligned__(16)));

using InterruptCallback = void (*)(void*);

struct InterruptHandler {
  InterruptCallback callback{};
  void* context{};
};

static InterruptHandler handler[channel_count];

static uint8_t next_channel;
static uint16_t enabled_channels;

static void init()
{
  static bool initialized;

  if(initialized) {
    return;
  }

  initialized = true;

  // Enable DMAC clock
  PM->AHBMASK.bit.DMAC_ = 1;
  PM->APBBMASK.bit.DMAC_ = 1;

  DMAC->BASEADDR.reg = (uintptr_t)_descriptor;
  DMAC->WRBADDR.reg = (uintptr_t)_write_back_descriptor;

  // Enable all priority levels
  DMAC->CTRL.bit.LVLEN0 = 1;
  DMAC->CTRL.bit.LVLEN1 = 1;
  DMAC->CTRL.bit.LVLEN2 = 1;
  DMAC->CTRL.bit.LVLEN3 = 1;

  NVIC_EnableIRQ(DMAC_IRQn);
}

Dma::Channel Dma::claim()
{
  init();

  if(next_channel >= channel_count) {
    NVIC_SystemReset();
  }

  return static_cast<Channel>(next_channel++);
}

DmacDescriptor& Dma::descriptor(Channel channel)
{
  return _descriptor[static_cast<uint8_t>(channel)];
}

DmacDescriptor& Dma::write_back_descriptor(Channel channel)
{
  return _write_back_descriptor[static_cast<uint8_t>(channel)];
}

void Dma::enable(Channel channel, uint8_t trigger_action, uint8_t trigger_source, uint8_t priority)
{
  Interrupts::critical_section([&]() {
    DMAC->CHID.bit.ID = static_cast<uint8_t>(channel);
    DMAC->CHCTRLB.bit.TRIGACT = trigger_action;
    DMAC->CHCTRLB.bit.TRIGSRC = trigger_source;
    DMAC->CHCTRLB.bit.LVL = priority;
    DMAC->CHCTRLA.bit.ENABLE = 1;
  });

  enabled_channels |= (1 << static_cast<uint8_t>(channel));

  DMAC->CTRL.bit.DMAENABLE = 1;
}

void Dma::disable(Channel channel)
{
  Interrupts::critical_section([&]() {
    DMAC->CHID.bit.ID = static_cast<uint8_t>(channel);
    DMAC->CHCTRLA.bit.ENABLE = 0;
  });

  enabled_channels &= ~(1 << static_cast<uint8_t>(channel));

  if(!enabled_channels) {
    DMAC->CTRL.bit.DMAENABLE = 0;
  }
}

void Dma::trigger(Channel channel)
{
  DMAC->SWTRIGCTRL.vec.SWTRIG |= (1 << static_cast<uint8_t>(channel));
}

void Dma::enable_interrupt(Channel channel)
{
  Interrupts::critical_section([&]() {
    DMAC->CHID.bit.ID = static_cast<uint8_t>(channel);
    DMAC->CHINTENSET.bit.TCMPL = 1;
  });
}

void Dma::disable_interrupt(Channel channel)
{
  Interrupts::critical_section([&]() {
    DMAC->CHID.bit.ID = static_cast<uint8_t>(channel);
    DMAC->CHINTENSET.bit.TCMPL = 0;
  });
}

void Dma::_install_interrupt_handler(Channel channel, void* context, void (*callback)(void*))
{
  handler[static_cast<uint8_t>(channel)] = InterruptHandler{ callback, context };
}

extern "C" void DMAC_Handler()
{
  for(uint8_t channel = 0; channel < next_channel; channel++) {
    bool transfer_complete = false;

    Interrupts::critical_section([&]() {
      DMAC->CHID.bit.ID = channel;

      if(DMAC->CHINTFLAG.bit.TCMPL) {
        DMAC->CHINTFLAG.reg = DMAC_CHINTENCLR_TCMPL;
        transfer_complete = true;
      }
    });

    if(transfer_complete && handler[channel].callback) {
      handler[channel].callback(handler[channel].context);
    }
  }
}
