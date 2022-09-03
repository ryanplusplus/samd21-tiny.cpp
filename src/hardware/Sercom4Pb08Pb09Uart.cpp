/*!
 * @file
 * @brief
 */

extern "C" {
#include "sam.h"
}

#include "Clock.hpp"
#include "Sercom4Pb08Pb09Uart.hpp"
#include "tiny/SingleSubscriberEvent.hpp"

#ifdef SERCOM4

static class : public tiny::IUart {
 public:
  void init(uint32_t baud)
  {
    // Enable SERCOM4 clock
    PM->APBCMASK.bit.SERCOM4_ = 1;

    // Select GCLK0 (DFLL48)
    GCLK->CLKCTRL.reg =
      GCLK_CLKCTRL_CLKEN |
      GCLK_CLKCTRL_GEN_GCLK0 |
      GCLK_CLKCTRL_ID(SERCOM4_GCLK_ID_CORE);
    while(GCLK->STATUS.bit.SYNCBUSY) {
    }

    PORT->Group[1].PMUX[8 >> 1].bit.PMUXE = MUX_PB08D_SERCOM4_PAD0;
    PORT->Group[1].PINCFG[8].bit.PMUXEN = 1;

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

    SERCOM4->USART.BAUD.reg = 0xFFFF - ((16ULL * 0xFFFF * baud) / Clock::gclk0_frequency);

    SERCOM4->USART.INTENSET.reg =
      SERCOM_USART_INTENSET_TXC |
      SERCOM_USART_INTENSET_RXC;

    NVIC_EnableIRQ(SERCOM4_IRQn);

    SERCOM4->USART.CTRLA.bit.ENABLE = 1;
    while(SERCOM4->USART.SYNCBUSY.bit.ENABLE) {
    }
  }

  void send(uint8_t byte) override
  {
    SERCOM4->USART.DATA.reg = byte;
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

void SERCOM4_Handler(void)
{
  if(SERCOM4->USART.INTFLAG.bit.TXC) {
    SERCOM4->USART.INTFLAG.reg |= SERCOM_USART_INTFLAG_TXC;
    instance.send_complete.publish();
  }

  while(SERCOM4->USART.INTFLAG.bit.RXC) {
    instance.receive.publish(SERCOM4->USART.DATA.reg);
  }
}

tiny::IUart& Sercom4Pb08Pb09Uart::get_instance(uint32_t baud)
{
  instance.init(baud);
  return instance;
}

#endif
