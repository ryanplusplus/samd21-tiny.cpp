/*!
 * @file
 * @brief
 */

#include "SercomUartBase.hpp"
#include "Sercom0Pa10Pa11Uart.hpp"

#ifdef SERCOM0

static constexpr Sercom* sercom()
{
  Sercom* sercom = SERCOM0;
  return sercom;
}

static void configure_pins()
{
  PORT->Group[0].PMUX[10 >> 1].bit.PMUXE = MUX_PA10C_SERCOM0_PAD2;
  PORT->Group[0].PINCFG[10].bit.PMUXEN = 1;

  PORT->Group[0].PMUX[11 >> 1].bit.PMUXO = MUX_PA11C_SERCOM0_PAD3;
  PORT->Group[0].PINCFG[11].bit.PMUXEN = 1;
}

static SercomUartBase<
  PM_APBCMASK_SERCOM0,
  SERCOM0_GCLK_ID_CORE,
  sercom,
  configure_pins,
  SERCOM_USART_CTRLA_TXPO(1), // SERCOM0_PAD2 used for TX
  SERCOM_USART_CTRLA_RXPO(3), // SERCOM0_PAD3 used for RX
  SERCOM0_IRQn>
  instance;

extern "C" void SERCOM0_Handler()
{
  instance.handle_isr();
}

tiny::IUart& Sercom0Pa10Pa11Uart::get_instance(uint32_t baud)
{
  instance.init(baud);
  return instance;
}

#endif
