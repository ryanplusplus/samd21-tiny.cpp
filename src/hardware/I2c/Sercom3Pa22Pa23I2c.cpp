/*!
 * @file
 * @brief
 */

#include "SercomI2cBase.hpp"
#include "Sercom3Pa22Pa23I2c.hpp"

#ifdef SERCOM3

static constexpr Sercom* sercom()
{
  Sercom* sercom = SERCOM3;
  return sercom;
}

static void configure_pins()
{
  PORT->Group[0].PMUX[22 >> 1].bit.PMUXE = MUX_PA22C_SERCOM3_PAD0;
  PORT->Group[0].PINCFG[22].bit.PMUXEN = 1;

  PORT->Group[0].PMUX[23 >> 1].bit.PMUXO = MUX_PA23C_SERCOM3_PAD1;
  PORT->Group[0].PINCFG[23].bit.PMUXEN = 1;
}

static SercomI2cBase<
  PM_APBCMASK_SERCOM3,
  SERCOM3_GCLK_ID_CORE,
  sercom,
  configure_pins>
  instance;

tiny::II2c& Sercom3Pa22Pa23I2c::get_instance()
{
  instance.init();
  return instance;
}

#endif
