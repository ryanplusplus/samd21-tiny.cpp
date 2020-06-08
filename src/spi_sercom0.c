/*!
 * @file
 * @brief
 */

#include "spi.h"
#include "spi_sercom0.h"

static struct spi_module spi;
static i_tiny_spi_t self;

static void transfer(
  i_tiny_spi_t* _self,
  const uint8_t* write_buffer,
  uint8_t* read_buffer,
  uint16_t buffer_size) {
  (void)_self;
}

static const i_tiny_spi_api_t api = { transfer };

i_tiny_spi_t* spi_sercom0_init(
  uint8_t cpol,
  uint8_t cpha,
  bool msb_first,
  uint32_t baud) {
  struct spi_config config = {
    .mode = SPI_MODE_MASTER,
    .data_order = msb_first ? SPI_DATA_ORDER_MSB : SPI_DATA_ORDER_LSB,
    .transfer_mode = (cpha << SERCOM_SPI_CTRLA_CPHA_Pos) | (cpol << SERCOM_SPI_CTRLA_CPOL_Pos),
    .mux_setting = SPI_SIGNAL_MUX_SETTING_D,
    .character_size = SPI_CHARACTER_SIZE_8BIT,
    .run_in_standby = false,
    .receiver_enable = true,
    .select_slave_low_detect_enable = true,
    .master_slave_select_enable = false,
    .generator_source = GCLK_GENERATOR_0,

    .mode_specific.master.baudrate = baud,

    .pinmux_pad0 = PINMUX_UNUSED,
    .pinmux_pad1 = PINMUX_PA05D_SERCOM0_PAD1,
    .pinmux_pad2 = PINMUX_PA06D_SERCOM0_PAD2,
    .pinmux_pad3 = PINMUX_PA07D_SERCOM0_PAD3
  };

  spi_init(&spi, SERCOM0, &config);

  spi_enable(&spi);

  self.api = &api;

  return &self;
}
