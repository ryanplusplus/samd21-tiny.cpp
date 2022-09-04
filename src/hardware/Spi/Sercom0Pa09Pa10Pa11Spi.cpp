/*!
 * @file
 * @brief
 */

#include "SercomSpiBase.hpp"
#include "Sercom0Pa09Pa10Pa11Spi.hpp"

#ifdef SERCOM0

static constexpr Sercom* sercom()
{
  Sercom* sercom = SERCOM0;
  return sercom;
}

static void configure_pins()
{
  PORT->Group[0].PMUX[9 >> 1].bit.PMUXO = MUX_PA09C_SERCOM0_PAD1;
  PORT->Group[0].PINCFG[9].bit.PMUXEN = 1;

  PORT->Group[0].PMUX[10 >> 1].bit.PMUXE = MUX_PA10C_SERCOM0_PAD2;
  PORT->Group[0].PINCFG[10].bit.PMUXEN = 1;

  PORT->Group[0].PMUX[11 >> 1].bit.PMUXO = MUX_PA11C_SERCOM0_PAD3;
  PORT->Group[0].PINCFG[11].bit.PMUXEN = 1;
}

static SercomSpiBase<
  PM_APBCMASK_SERCOM0,
  SERCOM0_GCLK_ID_CORE,
  sercom,
  configure_pins,
  SERCOM_SPI_CTRLA_DOPO(1), // SERCOM0_PAD2, 3 used for MOSI, SCK
  SERCOM_SPI_CTRLA_DIPO(1) // SERCOM0_PAD1 used for MISO
  >
  instance;

tiny::ISpi& Sercom0Pa09Pa10Pa11Spi::get_instance(
  bool cpol,
  bool cpha,
  bool msb_first,
  uint32_t baud)
{
  instance.init(cpol, cpha, msb_first, baud);
  return instance;
}

#endif
