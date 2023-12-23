/*!
 * @file
 * @brief
 */

#include "Sercom1Pa16Pa17I2c.hpp"
#include "SercomI2cBase.hpp"

#ifdef SERCOM1

static constexpr Sercom* sercom()
{
  Sercom* sercom = SERCOM1;
  return sercom;
}

static void configure_pins()
{
  PORT->Group[0].PMUX[16 >> 1].bit.PMUXE = MUX_PA16C_SERCOM1_PAD0;
  PORT->Group[0].PINCFG[16].bit.PMUXEN = 1;

  PORT->Group[0].PMUX[17 >> 1].bit.PMUXO = MUX_PA17C_SERCOM1_PAD1;
  PORT->Group[0].PINCFG[17].bit.PMUXEN = 1;
}

static SercomI2cBase<
  PM_APBCMASK_SERCOM1,
  SERCOM1_GCLK_ID_CORE,
  sercom,
  configure_pins>
  instance;

tiny::II2c& Sercom1Pa16Pa17I2c::get_instance()
{
  instance.init();
  return instance;
}

#endif
