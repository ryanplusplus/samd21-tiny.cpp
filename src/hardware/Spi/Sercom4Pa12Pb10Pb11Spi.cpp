/*!
 * @file
 * @brief
 */

extern "C" {
#include "sam.h"
}

#include "Sercom4Pa12Pb10Pb11Spi.hpp"
#include "Clock.hpp"
#include "tiny/utils.h"

#ifdef SERCOM4

using namespace tiny;

static class : public ISpi {
 public:
  void init(
    uint8_t cpol,
    uint8_t cpha,
    bool msb_first,
    uint32_t baud)
  {
    // Enable SERCOM4 clock
    PM->APBCMASK.bit.SERCOM4_ = 1;

    // Select GCLK0 (DFLL48)
    GCLK->CLKCTRL.reg =
      GCLK_CLKCTRL_CLKEN |
      GCLK_CLKCTRL_GEN_GCLK0 |
      GCLK_CLKCTRL_ID(SERCOM4_GCLK_ID_CORE);
    while(GCLK->STATUS.bit.SYNCBUSY) {
    }

    PORT->Group[0].PMUX[12 >> 1].bit.PMUXE = MUX_PA12D_SERCOM4_PAD0;
    PORT->Group[0].PINCFG[12].bit.PMUXEN = 1;

    PORT->Group[1].PMUX[10 >> 1].bit.PMUXE = MUX_PB10D_SERCOM4_PAD2;
    PORT->Group[1].PINCFG[10].bit.PMUXEN = 1;

    PORT->Group[1].PMUX[11 >> 1].bit.PMUXO = MUX_PB11D_SERCOM4_PAD3;
    PORT->Group[1].PINCFG[11].bit.PMUXEN = 1;

    SERCOM4->SPI.CTRLA.reg =
      SERCOM_SPI_CTRLA_MODE_SPI_MASTER |
      SERCOM_SPI_CTRLA_DOPO(1) | // SERCOM4_PAD2, 3 used for MOSI, SCK
      SERCOM_SPI_CTRLA_DIPO(0) | // SERCOM4_PAD0 used for MISO
      (cpha << SERCOM_SPI_CTRLA_CPHA_Pos) |
      (cpol << SERCOM_SPI_CTRLA_CPOL_Pos) |
      (msb_first ? 0 : SERCOM_SPI_CTRLA_DORD);

    SERCOM4->SPI.CTRLB.reg = SERCOM_SPI_CTRLB_RXEN;
    while(SERCOM4->SPI.SYNCBUSY.bit.CTRLB) {
    }

    SERCOM4->SPI.BAUD.reg = (Clock::gclk0_frequency - 2 * baud) / (2 * baud);

    SERCOM4->SPI.CTRLA.bit.ENABLE = 1;
    while(SERCOM4->SPI.SYNCBUSY.bit.ENABLE) {
    }
  }

  void transfer(
    const uint8_t* _write_buffer,
    uint8_t* _read_buffer,
    uint16_t buffer_size) override
  {
    auto write_buffer = reinterpret_cast<const uint8_t*>(_write_buffer);
    auto read_buffer = reinterpret_cast<uint8_t*>(_read_buffer);

    for(uint32_t i = 0; i < buffer_size; i++) {
      while(!SERCOM4->SPI.INTFLAG.bit.DRE) {
      }

      SERCOM4->SPI.DATA.reg = write_buffer[i];

      while(!SERCOM4->SPI.INTFLAG.bit.RXC) {
      }

      if(read_buffer) {
        read_buffer[i] = SERCOM4->SPI.DATA.reg;
      }
      else {
        uint8_t dummy_read = SERCOM4->SPI.DATA.reg;
        static_cast<void>(dummy_read);
      }
    }

    while(!SERCOM4->SPI.INTFLAG.bit.TXC) {
    }
  }
} instance;

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
