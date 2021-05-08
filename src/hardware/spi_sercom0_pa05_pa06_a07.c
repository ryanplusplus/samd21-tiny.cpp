/*!
 * @file
 * @brief
 */

#include "micro.h"
#include "clock.h"
#include "spi_sercom0_pa05_pa06_a07.h"

static void transfer(
  i_tiny_spi_t* self,
  const uint8_t* write_buffer,
  uint8_t* read_buffer,
  uint16_t buffer_size)
{
  (void)self;

  for(uint32_t i = 0; i < buffer_size; i++) {
    while(!SERCOM0->SPI.INTFLAG.bit.DRE) {
    }

    SERCOM0->SPI.DATA.reg = write_buffer[i];

    while(!SERCOM0->SPI.INTFLAG.bit.RXC) {
    }

    if(read_buffer) {
      read_buffer[i] = SERCOM0->SPI.DATA.reg;
    }
    else {
      uint8_t dummy_read = SERCOM0->SPI.DATA.reg;
      (void)dummy_read;
    }
  }

  while(!SERCOM0->SPI.INTFLAG.bit.TXC) {
  }
}

static inline void initialize_peripheral(bool cpol, bool cpha, bool msb_first, uint32_t baud)
{
  // Enable SERCOM0 clock
  PM->APBCMASK.reg |= PM_APBCMASK_SERCOM0;

  // Select GCLK0 (DFLL48)
  GCLK->CLKCTRL.reg =
    GCLK_CLKCTRL_CLKEN |
    GCLK_CLKCTRL_GEN_GCLK0 |
    GCLK_CLKCTRL_ID(SERCOM0_GCLK_ID_CORE);
  while(GCLK->STATUS.bit.SYNCBUSY) {
  }

  PORT->Group[0].PMUX[5 >> 1].bit.PMUXO = MUX_PA05D_SERCOM0_PAD1;
  PORT->Group[0].PINCFG[5].bit.PMUXEN = 1;

  PORT->Group[0].PMUX[6 >> 1].bit.PMUXE = MUX_PA06D_SERCOM0_PAD2;
  PORT->Group[0].PINCFG[6].bit.PMUXEN = 1;

  PORT->Group[0].PMUX[7 >> 1].bit.PMUXO = MUX_PA07D_SERCOM0_PAD3;
  PORT->Group[0].PINCFG[7].bit.PMUXEN = 1;

  SERCOM0->SPI.CTRLA.reg =
    SERCOM_SPI_CTRLA_MODE_SPI_MASTER |
    SERCOM_SPI_CTRLA_DOPO(1) | // SERCOM0_PAD2, 3 used for MOSI, SCK
    SERCOM_SPI_CTRLA_DIPO(1) | // SERCOM0_PAD1 used for MISO
    (cpha << SERCOM_SPI_CTRLA_CPHA_Pos) |
    (cpol << SERCOM_SPI_CTRLA_CPOL_Pos) |
    (msb_first ? 0 : SERCOM_SPI_CTRLA_DORD);

  SERCOM0->SPI.CTRLB.reg = SERCOM_SPI_CTRLB_RXEN;
  while(SERCOM0->SPI.SYNCBUSY.bit.CTRLB) {
  }

  SERCOM0->SPI.BAUD.reg = (clock_gclk0_frequency - 2 * baud) / (2 * baud);

  SERCOM0->SPI.CTRLA.bit.ENABLE = 1;
  while(SERCOM0->SPI.SYNCBUSY.bit.ENABLE) {
  }
}

static const i_tiny_spi_api_t api = { transfer };

i_tiny_spi_t* spi_sercom0_pa05_pa06_a07_init(bool cpol, bool cpha, bool msb_first, uint32_t baud)
{
  initialize_peripheral(cpol, cpha, msb_first, baud);

  static i_tiny_spi_t self;
  self.api = &api;
  return &self;
}
