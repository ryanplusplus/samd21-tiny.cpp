/*!
 * @file
 * @brief
 */

extern "C" {
#include "sam.h"
}

#include "Clock.hpp"

static inline void configure_bod33_for_48mhz(void)
{
  SYSCTRL->BOD33.bit.ENABLE = 0;
  while(!SYSCTRL->PCLKSR.bit.B33SRDY) {
  };

  // Configure the brown-out detector for 48 MHz
  SYSCTRL->BOD33.reg =
    SYSCTRL_BOD33_LEVEL(48) |
    SYSCTRL_BOD33_ACTION_NONE |
    SYSCTRL_BOD33_HYST;

  SYSCTRL->BOD33.bit.ENABLE = 1;
  while(!SYSCTRL->PCLKSR.bit.BOD33RDY) {
  }

  // Wait for 3.3ish volts
  while(SYSCTRL->PCLKSR.bit.BOD33DET) {
  }

  SYSCTRL->BOD33.bit.ENABLE = 0;
  while(!SYSCTRL->PCLKSR.bit.B33SRDY) {
  };

  // Now that we're at 3.3ish volts, we want to reset if
  // the VCC drops too low
  SYSCTRL->BOD33.bit.ACTION = SYSCTRL_BOD33_ACTION_RESET_Val;

  SYSCTRL->BOD33.bit.ENABLE = 1;
}

static inline void configure_flash_wait_states_for_48mhz(void)
{
  // 1 wait state required for 48 MHz at 3.3V
  NVMCTRL->CTRLB.bit.RWS = 1;
}

static inline void configure_osc32k(void)
{
  SYSCTRL->OSC32K.bit.WRTLOCK = 0;
  SYSCTRL->OSC32K.bit.STARTUP = 0x4;
  SYSCTRL->OSC32K.bit.ONDEMAND = 0;
  SYSCTRL->OSC32K.bit.RUNSTDBY = 0;
  SYSCTRL->OSC32K.bit.EN32K = 1;
  SYSCTRL->OSC32K.bit.ENABLE = 1;

  while(!SYSCTRL->PCLKSR.bit.OSC32KRDY) {
  }
}

static inline void configure_gclk1_with_osc32k_source_to_feed_dfll48(void)
{
  GCLK->GENDIV.reg = GCLK_GENDIV_ID(1) | GCLK_GENDIV_DIV(1);

  GCLK->GENCTRL.reg =
    GCLK_GENCTRL_ID(1) |
    GCLK_GENCTRL_IDC |
    GCLK_GENCTRL_GENEN |
    GCLK_GENCTRL_SRC_OSC32K;

  while(GCLK->STATUS.bit.SYNCBUSY) {
  }
}

static inline void configure_dfll48_with_gclk1_source(void)
{
  // Set GCLK1 (OSC32K) as the source for DFLL48
  GCLK->CLKCTRL.reg =
    GCLK_CLKCTRL_CLKEN |
    GCLK_CLKCTRL_GEN_GCLK1 |
    GCLK_CLKCTRL_ID(SYSCTRL_GCLK_ID_DFLL48);

  // This works around a quirk in the hardware (errata 1.2.1)
  // The DFLLCTRL register must be manually reset to this value before configuration
  while(!SYSCTRL->PCLKSR.bit.DFLLRDY) {
  }
  SYSCTRL->DFLLCTRL.reg = SYSCTRL_DFLLCTRL_ENABLE;
  while(!SYSCTRL->PCLKSR.bit.DFLLRDY) {
  }

  // Select middle of the road values for the fine and course steps
  SYSCTRL->DFLLMUL.bit.CSTEP = 31;
  SYSCTRL->DFLLMUL.bit.FSTEP = 511;

  // 32768 * 1465 = 48,005,120 Hz
  SYSCTRL->DFLLMUL.bit.MUL = 1465;

  while(!SYSCTRL->PCLKSR.bit.DFLLRDY) {
  }

  // Switch DFLL48M to Closed Loop mode and enable WAITLOCK
  SYSCTRL->DFLLCTRL.reg |= SYSCTRL_DFLLCTRL_MODE | SYSCTRL_DFLLCTRL_WAITLOCK;
}

static inline void configure_gclk0_with_dfll48_source(void)
{
  // No divisor for GCLK0
  GCLK->GENDIV.reg = GCLK_GENDIV_ID(0) | GCLK_GENDIV_DIV(1);

  GCLK->GENCTRL.reg =
    GCLK_GENCTRL_ID(0) |
    GCLK_GENCTRL_IDC |
    GCLK_GENCTRL_GENEN |
    GCLK_GENCTRL_SRC_DFLL48M;

  while(GCLK->STATUS.bit.SYNCBUSY) {
  }
}

static inline void configure_gclk2_with_osc32k_source(void)
{
  // No divisor for GCLK2
  GCLK->GENDIV.reg = GCLK_GENDIV_ID(2) | GCLK_GENDIV_DIV(1);

  GCLK->GENCTRL.reg =
    GCLK_GENCTRL_ID(2) |
    GCLK_GENCTRL_IDC |
    GCLK_GENCTRL_GENEN |
    GCLK_GENCTRL_SRC_OSC32K;

  while(GCLK->STATUS.bit.SYNCBUSY) {
  }
}

void Clock::init()
{
  configure_bod33_for_48mhz();
  configure_flash_wait_states_for_48mhz();
  configure_osc32k();
  configure_gclk1_with_osc32k_source_to_feed_dfll48();
  configure_dfll48_with_gclk1_source();
  configure_gclk0_with_dfll48_source();
  configure_gclk2_with_osc32k_source();
}
