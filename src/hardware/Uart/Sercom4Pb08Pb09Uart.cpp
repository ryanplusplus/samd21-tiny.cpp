/*!
 * @file
 * @brief
 */

#include "SercomUartBase.hpp"
#include "Sercom4Pb08Pb09Uart.hpp"

#ifdef SERCOM4

static constexpr Sercom* sercom()
{
  Sercom* sercom = SERCOM4;
  return sercom;
}

static void configure_pins()
{
  PORT->Group[1].PMUX[8 >> 1].bit.PMUXE = MUX_PB08D_SERCOM4_PAD0;
  PORT->Group[1].PINCFG[8].bit.PMUXEN = 1;

  PORT->Group[1].PMUX[9 >> 1].bit.PMUXO = MUX_PB09D_SERCOM4_PAD1;
  PORT->Group[1].PINCFG[9].bit.PMUXEN = 1;
}

SercomUartBase<
  PM_APBCMASK_SERCOM4,
  SERCOM4_GCLK_ID_CORE,
  sercom,
  configure_pins,
  SERCOM_USART_CTRLA_TXPO(0), // SERCOM4_PAD0 used for TX
  SERCOM_USART_CTRLA_RXPO(1), // SERCOM4_PAD1 used for RX
  SERCOM4_IRQn>
  instance;

extern "C" void SERCOM4_Handler()
{
  instance.handle_isr();
}

tiny::IUart& Sercom4Pb08Pb09Uart::get_instance(uint32_t baud)
{
  instance.init(baud);
  return instance;
}

#endif
