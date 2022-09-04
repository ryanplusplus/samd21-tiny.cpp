/*!
 * @file
 * @brief
 */

#include "SercomBufferedUartBase.hpp"
#include "Sercom0Pa06Pa07BufferedUart.hpp"

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

static SercomBufferedUartBase<
  PM_APBCMASK_SERCOM0,
  SERCOM0_GCLK_ID_CORE,
  sercom,
  configure_pins,
  SERCOM_USART_CTRLA_TXPO(1), // SERCOM0_PAD2 used for TX
  SERCOM_USART_CTRLA_RXPO(3), // SERCOM0_PAD3 used for RX
  SERCOM0_DMAC_ID_TX,
  SERCOM0_DMAC_ID_RX>
  instance;

tiny::IBufferedUart& Sercom0Pa06Pa07BufferedUart::get_instance(uint32_t baud)
{
  instance.init(baud);
  return instance;
}

#endif
