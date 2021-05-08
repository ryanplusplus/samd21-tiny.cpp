/*!
 * @file
 * @brief
 */

#include <stdint.h>
#include "micro.h"
#include "clock.h"
#include "uart_sercom4_pb08_pb09.h"
#include "tiny_event.h"

static i_tiny_uart_t self;
static tiny_event_t send_complete;
static tiny_event_t receive;

void SERCOM4_Handler(void)
{
  if(SERCOM4->USART.INTFLAG.bit.TXC) {
    SERCOM4->USART.INTFLAG.bit.TXC = 1;
    tiny_event_publish(&send_complete, NULL);
  }

  while(SERCOM4->USART.INTFLAG.bit.RXC) {
    const uint8_t byte = SERCOM4->USART.DATA.reg;
    tiny_event_publish(&receive, &(tiny_uart_on_receive_args_t){ .byte = byte });
  }
}

static void send(i_tiny_uart_t* self, uint8_t byte)
{
  (void)self;
  SERCOM4->USART.DATA.reg = byte;
}

static i_tiny_event_t* on_send_complete(i_tiny_uart_t* self)
{
  (void)self;
  return &send_complete.interface;
}

static i_tiny_event_t* on_receive(i_tiny_uart_t* self)
{
  (void)self;
  return &receive.interface;
}

static inline void initialize_peripheral(uint32_t baud)
{
  // Enable SERCOM4 clock
  PM->APBCMASK.reg |= PM_APBCMASK_SERCOM4;

  // Select GCLK0 (DFLL48)
  GCLK->CLKCTRL.reg =
    GCLK_CLKCTRL_CLKEN |
    GCLK_CLKCTRL_GEN_GCLK0 |
    GCLK_CLKCTRL_ID(SERCOM4_GCLK_ID_CORE);
  while(GCLK->STATUS.bit.SYNCBUSY) {
  }

  PORT->Group[1].OUTSET.reg = PORT_PB08;
  PORT->Group[1].DIRSET.reg = PORT_PB08;
  PORT->Group[1].PMUX[8 >> 1].bit.PMUXE = MUX_PB08D_SERCOM4_PAD0;
  PORT->Group[1].PINCFG[8].bit.PMUXEN = 1;

  PORT->Group[1].DIRSET.reg = PORT_PB09;
  PORT->Group[1].PMUX[9 >> 1].bit.PMUXO = MUX_PB09D_SERCOM4_PAD1;
  PORT->Group[1].PINCFG[9].bit.PMUXEN = 1;

  SERCOM4->USART.CTRLA.reg =
    SERCOM_USART_CTRLA_MODE_USART_INT_CLK | // Internal clock
    SERCOM_USART_CTRLA_TXPO(0) | // SERCOM4_PAD0 used for TX
    SERCOM_USART_CTRLA_RXPO(1) | // SERCOM4_PAD1 used for RX
    SERCOM_USART_CTRLA_DORD; // LSB first

  SERCOM4->USART.CTRLB.reg =
    SERCOM_USART_CTRLB_TXEN |
    SERCOM_USART_CTRLB_RXEN;
  while(SERCOM4->USART.SYNCBUSY.bit.CTRLB) {
  }

  SERCOM4->USART.BAUD.reg =
    0xFFFF - ((16 * 0xFFFF * baud) / clock_gclk0_frequency);

  SERCOM4->USART.INTENSET.reg =
    SERCOM_USART_INTENSET_TXC |
    SERCOM_USART_INTENSET_RXC;

  NVIC_EnableIRQ(SERCOM4_IRQn);

  SERCOM4->USART.CTRLA.bit.ENABLE = 1;
  while(SERCOM4->USART.SYNCBUSY.bit.ENABLE) {
  }
}

static const i_tiny_uart_api_t api = { send, on_send_complete, on_receive };

i_tiny_uart_t* uart_sercom4_pb08_pb09_init(uint32_t baud)
{
  tiny_event_init(&send_complete);
  tiny_event_init(&receive);

  initialize_peripheral(baud);

  self.api = &api;

  return &self;
}
