/*!
 * @file
 * @brief
 */

#include <stdint.h>
#include "micro.h"
#include "clock.h"
#include "uart_sercom0_pa10_pa11.h"
#include "tiny_event.h"

static i_tiny_uart_t self;
static tiny_event_t send_complete;
static tiny_event_t receive;

void SERCOM0_Handler(void)
{
  if(SERCOM0->USART.INTFLAG.bit.TXC) {
    SERCOM0->USART.INTFLAG.bit.TXC = 1;
    tiny_event_publish(&send_complete, NULL);
  }

  while(SERCOM0->USART.INTFLAG.bit.RXC) {
    const uint8_t byte = SERCOM0->USART.DATA.reg;
    tiny_event_publish(&receive, &(tiny_uart_on_receive_args_t){ .byte = byte });
  }
}

static void send(i_tiny_uart_t* self, uint8_t byte)
{
  (void)self;
  SERCOM0->USART.DATA.reg = byte;
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
  // Enable SERCOM0 clock
  PM->APBCMASK.reg |= PM_APBCMASK_SERCOM0;

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

  SERCOM0->USART.INTENSET.reg =
    SERCOM_USART_INTENSET_TXC |
    SERCOM_USART_INTENSET_RXC;

  NVIC_EnableIRQ(SERCOM0_IRQn);

  SERCOM0->USART.CTRLA.bit.ENABLE = 1;
  while(SERCOM0->USART.SYNCBUSY.bit.ENABLE) {
  }
}

static const i_tiny_uart_api_t api = { send, on_send_complete, on_receive };

i_tiny_uart_t* uart_sercom0_pa10_pa11_init(uint32_t baud)
{
  tiny_event_init(&send_complete);
  tiny_event_init(&receive);

  initialize_peripheral(baud);

  self.api = &api;

  return &self;
}
