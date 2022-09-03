/*!
 * @file
 * @brief
 */

extern "C" {
#include "sam.h"
}

#include "Clock.hpp"
#include "Sercom0Pa06Pa07Uart.hpp"
#include "tiny/SingleSubscriberEvent.hpp"

#ifdef SERCOM0

static class : public tiny::IUart {
 public:
  void init(uint32_t baud)
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

    PORT->Group[0].PMUX[6 >> 1].bit.PMUXE = MUX_PA06D_SERCOM0_PAD2;
    PORT->Group[0].PINCFG[6].bit.PMUXEN = 1;

    PORT->Group[0].PMUX[7 >> 1].bit.PMUXO = MUX_PA07D_SERCOM0_PAD3;
    PORT->Group[0].PINCFG[7].bit.PMUXEN = 1;

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

    SERCOM0->USART.BAUD.reg = 0xFFFF - ((16ULL * 0xFFFF * baud) / Clock::gclk0_frequency);

    SERCOM0->USART.INTENSET.reg =
      SERCOM_USART_INTENSET_TXC |
      SERCOM_USART_INTENSET_RXC;

    NVIC_EnableIRQ(SERCOM0_IRQn);

    SERCOM0->USART.CTRLA.bit.ENABLE = 1;
    while(SERCOM0->USART.SYNCBUSY.bit.ENABLE) {
    }
  }

  void send(uint8_t byte) override
  {
    SERCOM0->USART.DATA.reg = byte;
  }

  tiny::IEvent<>& on_send_complete() override
  {
    return send_complete;
  }

  tiny::IEvent<uint8_t>& on_receive() override
  {
    return receive;
  }

 public:
  tiny::SingleSubscriberEvent<> send_complete{};
  tiny::SingleSubscriberEvent<uint8_t> receive{};
} instance;

void SERCOM0_Handler(void)
{
  if(SERCOM0->USART.INTFLAG.bit.TXC) {
    SERCOM0->USART.INTFLAG.reg |= SERCOM_USART_INTFLAG_TXC;
    instance.send_complete.publish();
  }

  while(SERCOM0->USART.INTFLAG.bit.RXC) {
    instance.receive.publish(SERCOM0->USART.DATA.reg);
  }
}

tiny::IUart& Sercom0Pa06Pa07Uart::get_instance(uint32_t baud)
{
  instance.init(baud);
  return instance;
}

#endif
