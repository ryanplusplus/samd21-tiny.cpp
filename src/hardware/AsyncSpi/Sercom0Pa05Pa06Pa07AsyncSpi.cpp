/*!
 * @file
 * @brief
 */

#include "SercomAsyncSpiBase.hpp"
#include "Sercom0Pa05Pa06Pa07AsyncSpi.hpp"

#ifdef SERCOM0

static constexpr Sercom* sercom()
{
  Sercom* sercom = SERCOM0;
  return sercom;
}

static void configure_pins()
{
  PORT->Group[0].PMUX[5 >> 1].bit.PMUXO = MUX_PA05D_SERCOM0_PAD1;
  PORT->Group[0].PINCFG[5].bit.PMUXEN = 1;

  PORT->Group[0].PMUX[6 >> 1].bit.PMUXE = MUX_PA06D_SERCOM0_PAD2;
  PORT->Group[0].PINCFG[6].bit.PMUXEN = 1;

  PORT->Group[0].PMUX[7 >> 1].bit.PMUXO = MUX_PA07D_SERCOM0_PAD3;
  PORT->Group[0].PINCFG[7].bit.PMUXEN = 1;
}

static SercomAsyncSpiBase<
  PM_APBCMASK_SERCOM0,
  SERCOM0_GCLK_ID_CORE,
  sercom,
  configure_pins,
  SERCOM_SPI_CTRLA_DOPO(1), // SERCOM0_PAD2, 3 used for MOSI, SCK
  SERCOM_SPI_CTRLA_DIPO(1), // SERCOM0_PAD1 used for MISO
  SERCOM0_DMAC_ID_TX,
  SERCOM0_DMAC_ID_RX>
  instance;

tiny::IAsyncSpi& Sercom0Pa05Pa06Pa07AsyncSpi::get_instance(
  bool cpol,
  bool cpha,
  bool msb_first,
  uint32_t baud)
{
  instance.init(cpol, cpha, msb_first, baud);
  return instance;
}

#endif
