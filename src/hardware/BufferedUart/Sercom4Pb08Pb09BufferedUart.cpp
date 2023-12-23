/*!
 * @file
 * @brief
 */

#include "Sercom4Pb08Pb09BufferedUart.hpp"
#include "SercomBufferedUartBase.hpp"

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

static SercomBufferedUartBase<
  PM_APBCMASK_SERCOM4,
  SERCOM4_GCLK_ID_CORE,
  sercom,
  configure_pins,
  SERCOM_USART_CTRLA_TXPO(0), // SERCOM4_PAD0 used for TX
  SERCOM_USART_CTRLA_RXPO(1), // SERCOM4_PAD1 used for RX
  SERCOM4_DMAC_ID_TX,
  SERCOM4_DMAC_ID_RX>
  instance;

tiny::IBufferedUart& Sercom4Pb08Pb09BufferedUart::get_instance(uint32_t baud)
{
  instance.init(baud);
  return instance;
}

#endif
