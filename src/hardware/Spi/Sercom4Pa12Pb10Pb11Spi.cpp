/*!
 * @file
 * @brief
 */

#include "Sercom4Pa12Pb10Pb11Spi.hpp"
#include "SercomSpiBase.hpp"

#ifdef SERCOM4

static constexpr Sercom* sercom()
{
  Sercom* sercom = SERCOM4;
  return sercom;
}

static void configure_pins()
{
  PORT->Group[0].PMUX[12 >> 1].bit.PMUXE = MUX_PA12D_SERCOM4_PAD0;
  PORT->Group[0].PINCFG[12].bit.PMUXEN = 1;

  PORT->Group[1].PMUX[10 >> 1].bit.PMUXE = MUX_PB10D_SERCOM4_PAD2;
  PORT->Group[1].PINCFG[10].bit.PMUXEN = 1;

  PORT->Group[1].PMUX[11 >> 1].bit.PMUXO = MUX_PB11D_SERCOM4_PAD3;
  PORT->Group[1].PINCFG[11].bit.PMUXEN = 1;
}

static SercomSpiBase<
  PM_APBCMASK_SERCOM4,
  SERCOM4_GCLK_ID_CORE,
  sercom,
  configure_pins,
  SERCOM_SPI_CTRLA_DOPO(1), // SERCOM4_PAD2, 3 used for MOSI, SCK
  SERCOM_SPI_CTRLA_DIPO(0) // SERCOM4_PAD0 used for MISO
  >
  instance;

tiny::ISpi& Sercom4Pa12Pb10Pb11Spi::get_instance(
  bool cpol,
  bool cpha,
  bool msb_first,
  uint32_t baud)
{
  instance.init(cpol, cpha, msb_first, baud);
  return instance;
}

#endif
