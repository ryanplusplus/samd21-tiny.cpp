/*!
 * @file
 * @brief
 */

extern "C" {
#include "sam.h"
}

#include "Sercom4Pa12Pb10Pb11AsyncSpi.hpp"
#include "Clock.hpp"
#include "Dma.hpp"
#include "tiny/utils.h"

#ifdef SERCOM4

using namespace tiny;

static void* context;
static IAsyncSpi::Callback callback;

static class : public IAsyncSpi {
 public:
  void init(
    uint8_t cpol,
    uint8_t cpha,
    bool msb_first,
    uint32_t baud)
  {
    initialize_peripheral(cpol, cpha, msb_first, baud);
    configure_dma();
  }

  void initialize_peripheral(
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

  void configure_dma()
  {
    write_channel = Dma::claim();
    read_channel = Dma::claim();

    Dma::install_interrupt_handler(
      read_channel, +[]() {
        callback(context);
      });
    Dma::enable_interrupt(read_channel);
  }

  void transfer(
    const uint8_t* write_buffer,
    uint8_t* read_buffer,
    uint16_t buffer_size,
    void* _context,
    Callback _callback) override
  {
    context = _context;
    callback = _callback;

    if(write_buffer && read_buffer) {
      configure_read_channel(read_buffer, buffer_size);
      configure_write_channel(write_buffer, buffer_size);
    }
    else if(write_buffer) {
      configure_read_channel(&dummy_read_buffer, 1);
      configure_write_channel(write_buffer, buffer_size);
    }
    else if(read_buffer) {
      configure_read_channel(read_buffer, buffer_size);
      configure_write_channel(read_buffer, buffer_size);
    }
  }

  void configure_read_channel(const void* buffer, uint16_t buffer_size)
  {
    auto& d = Dma::descriptor(read_channel);
    d.BTCTRL.bit.STEPSIZE = DMAC_BTCTRL_STEPSIZE_X1_Val;
    d.BTCTRL.bit.STEPSEL = DMAC_BTCTRL_STEPSEL_DST_Val;
    d.BTCTRL.bit.SRCINC = 0;
    d.BTCTRL.bit.BEATSIZE = DMAC_BTCTRL_BEATSIZE_BYTE_Val;
    d.BTCTRL.bit.BLOCKACT = DMAC_BTCTRL_BLOCKACT_NOACT_Val;
    d.BTCTRL.bit.VALID = 1;
    d.BTCNT.bit.BTCNT = buffer_size;
    d.SRCADDR.bit.SRCADDR = reinterpret_cast<uintptr_t>(&SERCOM4->SPI.DATA.reg);
    d.DESCADDR.bit.DESCADDR = 0;

    if(buffer_size > 1) {
      d.BTCTRL.bit.DSTINC = 1;
      d.DSTADDR.bit.DSTADDR = reinterpret_cast<uintptr_t>(buffer) + buffer_size;
    }
    else {
      d.BTCTRL.bit.DSTINC = 0;
      d.DSTADDR.bit.DSTADDR = reinterpret_cast<uintptr_t>(buffer);
    }

    Dma::enable(
      read_channel,
      DMAC_CHCTRLB_TRIGACT_BEAT_Val,
      SERCOM4_DMAC_ID_RX,
      DMAC_CHCTRLB_LVL_LVL1_Val);
  }

  void configure_write_channel(const void* buffer, uint16_t buffer_size)
  {
    auto& d = Dma::descriptor(write_channel);
    d.BTCTRL.bit.STEPSIZE = DMAC_BTCTRL_STEPSIZE_X1_Val;
    d.BTCTRL.bit.STEPSEL = DMAC_BTCTRL_STEPSEL_SRC_Val;
    d.BTCTRL.bit.DSTINC = 0;
    d.BTCTRL.bit.SRCINC = 1;
    d.BTCTRL.bit.BEATSIZE = DMAC_BTCTRL_BEATSIZE_BYTE_Val;
    d.BTCTRL.bit.BLOCKACT = DMAC_BTCTRL_BLOCKACT_NOACT_Val;
    d.BTCTRL.bit.VALID = 1;
    d.BTCNT.bit.BTCNT = buffer_size;
    d.SRCADDR.bit.SRCADDR = reinterpret_cast<uintptr_t>(buffer) + buffer_size;
    d.DSTADDR.bit.DSTADDR = reinterpret_cast<uintptr_t>(&SERCOM4->SPI.DATA.reg);
    d.DESCADDR.bit.DESCADDR = 0;

    Dma::enable(
      write_channel,
      DMAC_CHCTRLB_TRIGACT_BEAT_Val,
      SERCOM4_DMAC_ID_TX,
      DMAC_CHCTRLB_LVL_LVL0_Val);
  }

  Dma::Channel write_channel{};
  Dma::Channel read_channel{};

  uint8_t dummy_read_buffer{};
} instance;

tiny::IAsyncSpi& Sercom4Pa12Pb10Pb11AsyncSpi::get_instance(
  bool cpol,
  bool cpha,
  bool msb_first,
  uint32_t baud)
{
  instance.init(cpol, cpha, msb_first, baud);
  return instance;
}

#endif
