/*!
 * @file
 * @brief
 */

#ifndef SercomSpiBase_hpp
#define SercomSpiBase_hpp

extern "C" {
#include "sam.h"
}

#include "Clock.hpp"
#include "tiny/hal/ISpi.hpp"

template <
  uint32_t pm_mask,
  uint8_t gclk_id,
  Sercom* sercom(),
  void configure_pins(),
  uint32_t dopo,
  uint32_t dipo>
class SercomSpiBase : public tiny::ISpi {
 public:
  void init(
    uint8_t cpol,
    uint8_t cpha,
    bool msb_first,
    uint32_t baud)
  {
    // Enable SERCOM clock
    PM->APBCMASK.reg |= pm_mask;

    // Select GCLK0 (DFLL48)
    GCLK->CLKCTRL.reg =
      GCLK_CLKCTRL_CLKEN |
      GCLK_CLKCTRL_GEN_GCLK0 |
      GCLK_CLKCTRL_ID(gclk_id);
    while(GCLK->STATUS.bit.SYNCBUSY) {
    }

    configure_pins();

    sercom()->SPI.CTRLA.reg =
      SERCOM_SPI_CTRLA_MODE_SPI_MASTER |
      dopo |
      dipo |
      (cpha << SERCOM_SPI_CTRLA_CPHA_Pos) |
      (cpol << SERCOM_SPI_CTRLA_CPOL_Pos) |
      (msb_first ? 0 : SERCOM_SPI_CTRLA_DORD);

    sercom()->SPI.CTRLB.reg = SERCOM_SPI_CTRLB_RXEN;
    while(sercom()->SPI.SYNCBUSY.bit.CTRLB) {
    }

    sercom()->SPI.BAUD.reg = (Clock::gclk0_frequency - 2 * baud) / (2 * baud);

    sercom()->SPI.CTRLA.bit.ENABLE = 1;
    while(sercom()->SPI.SYNCBUSY.bit.ENABLE) {
    }
  }

 public:
  void transfer(
    const uint8_t* _write_buffer,
    uint8_t* _read_buffer,
    uint16_t buffer_size) override
  {
    auto write_buffer = reinterpret_cast<const uint8_t*>(_write_buffer);
    auto read_buffer = reinterpret_cast<uint8_t*>(_read_buffer);

    for(uint32_t i = 0; i < buffer_size; i++) {
      while(!sercom()->SPI.INTFLAG.bit.DRE) {
      }

      sercom()->SPI.DATA.reg = write_buffer[i];

      while(!sercom()->SPI.INTFLAG.bit.RXC) {
      }

      if(read_buffer) {
        read_buffer[i] = sercom()->SPI.DATA.reg;
      }
      else {
        uint8_t dummy_read = sercom()->SPI.DATA.reg;
        static_cast<void>(dummy_read);
      }
    }

    while(!sercom()->SPI.INTFLAG.bit.TXC) {
    }
  }
};

#endif
