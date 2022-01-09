/*!
 * @file
 * @brief
 */

#include <stdbool.h>
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

static bool send_completed;

// fixme
// void SERCOM0_Handler(void)
// {
//   if(SERCOM0->USART.INTFLAG.bit.TXC) {
//     SERCOM0->USART.INTFLAG.reg |= SERCOM_USART_INTFLAG_TXC;
//     tiny_event_publish(&send_complete, NULL);
//   }

//   while(SERCOM0->USART.INTFLAG.bit.RXC) {
//     const uint8_t byte = SERCOM0->USART.DATA.reg;
//     tiny_event_publish(&receive, NULL);
//     (void)byte;
//   }
// }

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

  dma_channel_trigger(send_channel);
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

static void run(i_tiny_buffered_uart_t* self)
{
  (void)self;

  if(send_completed) {
    send_completed = false;
    tiny_event_publish(&send_complete, NULL);
  }

  // check for receive complete
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

  SERCOM0->USART.BAUD.reg =
    0xFFFF - ((16 * 0xFFFF * baud) / clock_gclk0_frequency);

  // fixme
  // {
  //   SERCOM0->USART.INTENSET.reg =
  //     SERCOM_USART_INTENSET_TXC |
  //     SERCOM_USART_INTENSET_RXC;

  //   NVIC_EnableIRQ(SERCOM0_IRQn);
  // }

  SERCOM0->USART.CTRLA.bit.ENABLE = 1;
  while(SERCOM0->USART.SYNCBUSY.bit.ENABLE) {
  }
}

static void dma_send_complete(void)
{
  send_completed = true;
}

static void initialize_send_channel(void)
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

static void initialize_receive_channel(void)
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
    DMAC_CHCTRLB_LVL_LVL1_Val);
}

static const i_tiny_buffered_uart_api_t api = { send, on_send_complete, on_receive, run };

i_tiny_buffered_uart_t* buffered_uart_sercom0_pa10_pa11_init(uint32_t baud)
{
  tiny_event_init(&send_complete);
  tiny_event_init(&receive);

  initialize_peripheral(baud);
  initialize_send_channel();
  initialize_receive_channel();

  static i_tiny_buffered_uart_t self;
  self.api = &api;
  return &self;
}
