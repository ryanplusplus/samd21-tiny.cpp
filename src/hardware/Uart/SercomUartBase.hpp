/*!
 * @file
 * @brief
 */

#ifndef SercomUartBasep_hpp
#define SercomUartBasep_hpp

extern "C" {
#include "sam.h"
}

#include "Clock.hpp"
#include "tiny/hal/IUart.hpp"
#include "tiny/SingleSubscriberEvent.hpp"

template <uint32_t pm_mask, uint8_t gclk_id, Sercom* sercom(), void configure_pins(), uint32_t txpo, uint32_t rxpo, IRQn_Type irq>
class SercomUartBase : public tiny::IUart {
 public:
  void init(uint32_t baud)
  {
    // Enable SERCOM clock
    PM->APBCMASK.reg |= pm_mask;

    // Select GCLK0 (DFLL48)
    GCLK->CLKCTRL.reg =
      GCLK_CLKCTRL_CLKEN |
      GCLK_CLKCTRL_GEN_GCLK0 |
      GCLK_CLKCTRL_ID(gclk_id);
    while(GCLK->STATUS.bit.SYNCBUSY) {
    }

    configure_pins();

    sercom()->USART.CTRLA.reg =
      SERCOM_USART_CTRLA_MODE_USART_INT_CLK | // Internal clock
      txpo |
      rxpo |
      SERCOM_USART_CTRLA_DORD; // LSB first

    sercom()->USART.CTRLB.reg =
      SERCOM_USART_CTRLB_TXEN |
      SERCOM_USART_CTRLB_RXEN;
    while(sercom()->USART.SYNCBUSY.bit.CTRLB) {
    }

    sercom()->USART.BAUD.reg = 0xFFFF - ((16ULL * 0xFFFF * baud) / Clock::gclk0_frequency);

    sercom()->USART.INTENSET.reg =
      SERCOM_USART_INTENSET_TXC |
      SERCOM_USART_INTENSET_RXC;

    NVIC_EnableIRQ(irq);

    sercom()->USART.CTRLA.bit.ENABLE = 1;
    while(sercom()->USART.SYNCBUSY.bit.ENABLE) {
    }
  }

  void handle_isr()
  {
    if(sercom()->USART.INTFLAG.bit.TXC) {
      sercom()->USART.INTFLAG.reg |= SERCOM_USART_INTFLAG_TXC;
      send_complete.publish();
    }

    while(sercom()->USART.INTFLAG.bit.RXC) {
      receive.publish(sercom()->USART.DATA.reg);
    }
  }

  void send(uint8_t byte) override
  {
    sercom()->USART.DATA.reg = byte;
  }

  tiny::IEvent<>& on_send_complete() override
  {
    return send_complete;
  }

  tiny::IEvent<uint8_t>& on_receive() override
  {
    return receive;
  }

 private:
  tiny::SingleSubscriberEvent<> send_complete{};
  tiny::SingleSubscriberEvent<uint8_t> receive{};
};

#endif
