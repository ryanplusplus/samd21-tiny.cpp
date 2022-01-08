/*!
 * @file
 * @brief
 */

#ifndef dma_h
#define dma_h

#include <stdint.h>
#include "sam.h"

void dma_init(void);
// fixme could return channel instead of taking it as an arg
void dma_claim_channel(uint8_t channel);
DmacDescriptor* dma_channel_descriptor(uint8_t channel);
DmacDescriptor* dma_channel_write_back_descriptor(uint8_t channel);
void dma_enable_channel(
  uint8_t channel,
  uint8_t trigger_action,
  uint8_t trigger_source,
  uint8_t priority);
void dma_disable_channel(uint8_t channel);
void dma_trigger_channel(uint8_t channel);

#endif
