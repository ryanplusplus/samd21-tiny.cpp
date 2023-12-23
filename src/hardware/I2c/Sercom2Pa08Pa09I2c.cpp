/*!
 * @file
 * @brief
 */

#include "Sercom2Pa08Pa09I2c.hpp"
#include "SercomI2cBase.hpp"

#ifdef SERCOM2

static constexpr Sercom* sercom()
{
  Sercom* sercom = SERCOM2;
  return sercom;
}

static void configure_pins()
{
  PORT->Group[0].PMUX[8 >> 1].bit.PMUXE = MUX_PA08D_SERCOM2_PAD0;
  PORT->Group[0].PINCFG[8].bit.PMUXEN = 1;

  PORT->Group[0].PMUX[9 >> 1].bit.PMUXO = MUX_PA09D_SERCOM2_PAD1;
  PORT->Group[0].PINCFG[9].bit.PMUXEN = 1;
}

static SercomI2cBase<
  PM_APBCMASK_SERCOM2,
  SERCOM2_GCLK_ID_CORE,
  sercom,
  configure_pins>
  instance;

tiny::II2c& Sercom2Pa08Pa09I2c::get_instance()
{
  instance.init();
  return instance;
}

#endif
