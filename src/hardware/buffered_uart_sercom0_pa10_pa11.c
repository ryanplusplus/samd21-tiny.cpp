/*!
 * @file
 * @brief
 */

#include <stdbool.h>
#include <string.h>
#include "sam.h"
#include "clock.h"
#include "dma.h"
#include "buffered_uart_sercom0_pa10_pa11.h"
#include "tiny_event.h"
#include "tiny_utils.h"

enum {
  receive_buffer_size = 100
};

static tiny_event_t send_complete;
static tiny_event_t receive;

static uint8_t send_channel;
static uint8_t receive_channel;

static uint8_t receive_buffer[receive_buffer_size];
static uint16_t receive_tail;

static bool send_completed;

static void send(i_tiny_buffered_uart_t* self, const void* buffer, uint16_t buffer_size)
{
  (void)self;

  DmacDescriptor* d = dma_channel_descriptor(send_channel);
  d->BTCNT.bit.BTCNT = buffer_size;
  d->SRCADDR.bit.SRCADDR = (uintptr_t)buffer + buffer_size;
  d->DSTADDR.bit.DSTADDR = (uintptr_t)&SERCOM0->USART.DATA.reg;

  dma_channel_enable(
    send_channel,
    DMAC_CHCTRLB_TRIGACT_BEAT_Val,
    SERCOM0_DMAC_ID_TX,
    DMAC_CHCTRLB_LVL_LVL0_Val);
}

static i_tiny_event_t* on_send_complete(i_tiny_buffered_uart_t* self)
{
  (void)self;
  return &send_complete.interface;
}

static i_tiny_event_t* on_receive(i_tiny_buffered_uart_t* self)
{
  (void)self;
  return &receive.interface;
}

static inline uint16_t _receive_head(void)
{
  DMAC_ACTIVE_Type active = DMAC->ACTIVE;
  uint16_t receive_dma_beat_count;

  if(active.bit.ID == receive_channel) {
    receive_dma_beat_count = active.bit.BTCNT;
  }
  else {
    receive_dma_beat_count = dma_channel_write_back_descriptor(receive_channel)->BTCNT.reg;
  }

  return receive_buffer_size - receive_dma_beat_count;
}

static inline void handle_receive_ready(void)
{
  uint16_t receive_head = _receive_head();

  if(receive_head != receive_tail) {
    uint8_t publication_buffer[receive_buffer_size];
    uint16_t count;

    if(receive_tail > receive_head) {
      memcpy(publication_buffer, receive_buffer + receive_tail, receive_buffer_size - receive_tail);
      memcpy(publication_buffer + (receive_buffer_size - receive_tail), receive_buffer, receive_head);
      count = receive_buffer_size - receive_tail + receive_head;
    }
    else {
      memcpy(publication_buffer, receive_buffer + receive_tail, receive_head - receive_tail);
      count = receive_head - receive_tail;
    }

    receive_tail = receive_head;

    tiny_buffered_uart_on_receive_args_t args = {
      .buffer = publication_buffer,
      .buffer_size = count,
    };
    tiny_event_publish(&receive, &args);
  }
}

static inline void handle_send_complete(void)
{
  if(send_completed) {
    send_completed = false;
    tiny_event_publish(&send_complete, NULL);
  }
}

static void run(i_tiny_buffered_uart_t* self)
{
  (void)self;
  handle_receive_ready();
  handle_send_complete();
}

static inline void initialize_peripheral(uint32_t baud)
{
  // Enable SERCOM0 clock
  PM->APBCMASK.bit.SERCOM0_ = 1;

  // Select GCLK0 (DFLL48)
  GCLK->CLKCTRL.reg =
    GCLK_CLKCTRL_CLKEN |
    GCLK_CLKCTRL_GEN_GCLK0 |
    GCLK_CLKCTRL_ID(SERCOM0_GCLK_ID_CORE);
  while(GCLK->STATUS.bit.SYNCBUSY) {
  }

  PORT->Group[0].PMUX[10 >> 1].bit.PMUXE = MUX_PA10C_SERCOM0_PAD2;
  PORT->Group[0].PINCFG[10].bit.PMUXEN = 1;

  PORT->Group[0].PMUX[11 >> 1].bit.PMUXO = MUX_PA11C_SERCOM0_PAD3;
  PORT->Group[0].PINCFG[11].bit.PMUXEN = 1;

  SERCOM0->USART.CTRLA.reg =
    SERCOM_USART_CTRLA_MODE_USART_INT_CLK | // Internal clock
    SERCOM_USART_CTRLA_TXPO(1) | // SERCOM0_PAD2 used for TX
    SERCOM_USART_CTRLA_RXPO(3) | // SERCOM0_PAD3 used for RX
    SERCOM_USART_CTRLA_DORD; // LSB first

  SERCOM0->USART.CTRLB.reg =
    SERCOM_USART_CTRLB_TXEN |
    SERCOM_USART_CTRLB_RXEN;
  while(SERCOM0->USART.SYNCBUSY.bit.CTRLB) {
  }

  SERCOM0->USART.BAUD.reg = 0xFFFF - ((16ULL * 0xFFFF * baud) / clock_gclk0_frequency);

  SERCOM0->USART.CTRLA.bit.ENABLE = 1;
  while(SERCOM0->USART.SYNCBUSY.bit.ENABLE) {
  }
}

static void dma_send_complete(void)
{
  send_completed = true;
}

static inline void configure_receive_channel(void)
{
  receive_channel = dma_channel_claim();

  DmacDescriptor* d = dma_channel_descriptor(receive_channel);
  d->BTCTRL.bit.STEPSIZE = DMAC_BTCTRL_STEPSIZE_X1_Val;
  d->BTCTRL.bit.STEPSEL = DMAC_BTCTRL_STEPSEL_DST_Val;
  d->BTCTRL.bit.DSTINC = 1;
  d->BTCTRL.bit.SRCINC = 0;
  d->BTCTRL.bit.BEATSIZE = DMAC_BTCTRL_BEATSIZE_BYTE_Val;
  d->BTCTRL.bit.BLOCKACT = DMAC_BTCTRL_BLOCKACT_NOACT_Val;
  d->BTCTRL.bit.VALID = 1;
  d->BTCNT.bit.BTCNT = receive_buffer_size;
  d->SRCADDR.bit.SRCADDR = (uintptr_t)&SERCOM0->USART.DATA.reg;
  d->DSTADDR.bit.DSTADDR = (uintptr_t)(receive_buffer + receive_buffer_size);
  d->DESCADDR.bit.DESCADDR = (uintptr_t)d;

  dma_channel_enable(
    receive_channel,
    DMAC_CHCTRLB_TRIGACT_BEAT_Val,
    SERCOM0_DMAC_ID_RX,
    DMAC_CHCTRLB_LVL_LVL3_Val);
}

static inline void configure_send_channel(void)
{
  send_channel = dma_channel_claim();

  DmacDescriptor* d = dma_channel_descriptor(send_channel);
  d->BTCTRL.bit.STEPSIZE = DMAC_BTCTRL_STEPSIZE_X1_Val;
  d->BTCTRL.bit.STEPSEL = DMAC_BTCTRL_STEPSEL_SRC_Val;
  d->BTCTRL.bit.DSTINC = 0;
  d->BTCTRL.bit.SRCINC = 1;
  d->BTCTRL.bit.BEATSIZE = DMAC_BTCTRL_BEATSIZE_BYTE_Val;
  d->BTCTRL.bit.BLOCKACT = DMAC_BTCTRL_BLOCKACT_NOACT_Val;
  d->BTCTRL.bit.VALID = 1;
  d->DESCADDR.bit.DESCADDR = 0;

  dma_channel_install_interrupt_handler(send_channel, dma_send_complete);
  dma_channel_enable_interrupt(send_channel);
}

static const i_tiny_buffered_uart_api_t api = { send, on_send_complete, on_receive, run };

i_tiny_buffered_uart_t* buffered_uart_sercom0_pa10_pa11_init(uint32_t baud)
{
  tiny_event_init(&send_complete);
  tiny_event_init(&receive);

  initialize_peripheral(baud);
  configure_receive_channel();
  configure_send_channel();

  static i_tiny_buffered_uart_t self;
  self.api = &api;
  return &self;
}
