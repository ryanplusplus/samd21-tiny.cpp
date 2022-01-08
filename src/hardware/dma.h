/*!
 * @file
 * @brief
 */

#ifndef dma_h
#define dma_h

#include <stdint.h>
#include "sam.h"

typedef void (*dma_interrupt_handler_t)(void);

void dma_init(void);
uint8_t dma_channel_claim(void);
DmacDescriptor* dma_channel_descriptor(uint8_t channel);
DmacDescriptor* dma_channel_write_back_descriptor(uint8_t channel);
void dma_channel_enable(uint8_t channel, uint8_t trigger_action, uint8_t trigger_source, uint8_t priority);
void dma_channel_disable(uint8_t channel);
void dma_channel_trigger(uint8_t channel);
void dma_channel_enable_interrupt(uint8_t channel);
void dma_channel_disable_interrupt(uint8_t channel);
void dma_channel_install_interrupt_handler(uint8_t channel, dma_interrupt_handler_t handler);

#endif
