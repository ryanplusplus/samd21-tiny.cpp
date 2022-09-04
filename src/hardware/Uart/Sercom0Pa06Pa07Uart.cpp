/*!
 * @file
 * @brief
 */

#include "SercomUartBase.hpp"
#include "Sercom0Pa06Pa07Uart.hpp"

#ifdef SERCOM0

static constexpr Sercom* sercom()
{
  Sercom* sercom = SERCOM0;
  return sercom;
}

static void configure_pins()
{
  PORT->Group[0].PMUX[6 >> 1].bit.PMUXE = MUX_PA06D_SERCOM0_PAD2;
  PORT->Group[0].PINCFG[6].bit.PMUXEN = 1;

  PORT->Group[0].PMUX[7 >> 1].bit.PMUXO = MUX_PA07D_SERCOM0_PAD3;
  PORT->Group[0].PINCFG[7].bit.PMUXEN = 1;
}

SercomUartBase<
  sercom,
  SERCOM0_GCLK_ID_CORE,
  configure_pins,
  SERCOM_USART_CTRLA_TXPO(1),
  SERCOM_USART_CTRLA_RXPO(3),
  SERCOM0_IRQn>
  instance;

extern "C" void SERCOM0_Handler()
{
  instance.handle_isr();
}

tiny::IUart& Sercom0Pa06Pa07Uart::get_instance(uint32_t baud)
{
  instance.init(baud);
  return instance;
}

#endif
