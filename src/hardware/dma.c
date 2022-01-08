/*!
 * @file
 * @brief
 */

#include <stdbool.h>
#include "dma_configuration.h"
#include "dma.h"
#include "sam.h"
#include "tiny_utils.h"

enum {
  maximum_channel_count = 12
};

tiny_static_assert(dma_channel_count <= maximum_channel_count);

static DmacDescriptor descriptor[dma_channel_count];
static DmacDescriptor write_back_descriptor[dma_channel_count];
static bool used[dma_channel_count];

void dma_init(void)
{
  // Enable DMAC clock
  PM->AHBMASK.bit.DMAC_ = 1;

  DMAC->BASEADDR.reg = (uintptr_t)descriptor;
  DMAC->WRBADDR.reg = (uintptr_t)write_back_descriptor;

  // Enable all priority levels
  DMAC->CTRL.bit.LVLEN0 = 1;
  DMAC->CTRL.bit.LVLEN1 = 1;
  DMAC->CTRL.bit.LVLEN2 = 1;
  DMAC->CTRL.bit.LVLEN3 = 1;
}

void dma_claim_channel(uint8_t channel)
{
  if(used[channel] || (channel >= dma_channel_count)) {
    NVIC_SystemReset();
  }

  used[channel] = true;
}

DmacDescriptor* dma_descriptor(uint8_t channel)
{
  return &descriptor[channel];
}

DmacDescriptor* dma_write_back_descriptor(uint8_t channel)
{
  return &write_back_descriptor[channel];
}

// fixme use typedefs for priority, trigger action, and trigger source?
void dma_enable_channel(
  uint8_t channel,
  uint8_t trigger_action,
  uint8_t trigger_source,
  uint8_t priority)
{
  DMAC->CTRL.bit.DMAENABLE = 1;

  DMAC->CHID.bit.ID = channel;
  DMAC->CHCTRLB.bit.TRIGACT = trigger_action;
  DMAC->CHCTRLB.bit.TRIGSRC = trigger_source;
  DMAC->CHCTRLB.bit.LVL = priority;
  DMAC->CHCTRLA.bit.ENABLE = 1;
}

void dma_disable_channel(uint8_t channel)
{
  DMAC->CHID.bit.ID = channel;
  DMAC->CHCTRLA.bit.ENABLE = 0;
}

// CHINTENCLR.TCMPL -> transfer complete interrupt
