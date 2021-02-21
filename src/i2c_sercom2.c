/*!
 * @file
 * @brief
 */

#include "i2c_sercom2.h"
#include "i2c_master.h"

static struct i2c_master_module i2c;
static i_tiny_i2c_t self;

static bool write(
  i_tiny_i2c_t* _self,
  uint8_t address,
  bool prepare_for_restart,
  const uint8_t* buffer,
  uint16_t buffer_size)
{
  (void)_self;

  struct i2c_master_packet packet = {
    .address = address,
    .data_length = buffer_size,
    .data = (uint8_t*)buffer,
    .ten_bit_address = false,
    .high_speed = false,
    .hs_master_code = 0
  };

  if(prepare_for_restart) {
    return !i2c_master_write_packet_wait_no_stop(&i2c, &packet);
  }
  else {
    return !i2c_master_write_packet_wait(&i2c, &packet);
  }
}

static bool read(
  i_tiny_i2c_t* self,
  uint8_t address,
  bool prepare_for_restart,
  uint8_t* buffer,
  uint16_t buffer_size)
{
  (void)self;

  struct i2c_master_packet packet = {
    .address = address,
    .data_length = buffer_size,
    .data = buffer,
    .ten_bit_address = false,
    .high_speed = false,
    .hs_master_code = 0
  };

  if(prepare_for_restart) {
    return !i2c_master_read_packet_wait_no_stop(&i2c, &packet);
  }
  else {
    return !i2c_master_read_packet_wait(&i2c, &packet);
  }
}

static void reset(i_tiny_i2c_t* self)
{
  (void)self;
  i2c_master_reset(&i2c);
}

static const i_tiny_i2c_api_t api = { write, read, reset };

i_tiny_i2c_t* i2c_sercom2_init(void)
{
  struct i2c_master_config config = {
    .baud_rate = I2C_MASTER_BAUD_RATE_100KHZ,
    .baud_rate_high_speed = I2C_MASTER_BAUD_RATE_3400KHZ,
    .transfer_speed = I2C_MASTER_SPEED_STANDARD_AND_FAST,
    .generator_source = GCLK_GENERATOR_0,
    .run_in_standby = false,
    .start_hold_time = I2C_MASTER_START_HOLD_TIME_300NS_600NS,
    .buffer_timeout = 65535,
    .unknown_bus_state_timeout = 65535,
    .pinmux_pad0 = PINMUX_PA08D_SERCOM2_PAD0,
    .pinmux_pad1 = PINMUX_PA09D_SERCOM2_PAD1,
    .scl_low_timeout = false,
    .inactive_timeout = I2C_MASTER_INACTIVE_TIMEOUT_DISABLED,
    .scl_stretch_only_after_ack_bit = false,
    .slave_scl_low_extend_timeout = false,
    .master_scl_low_extend_timeout = false
  };

  i2c_master_init(&i2c, SERCOM2, &config);
  i2c_master_enable(&i2c);

  self.api = &api;

  return &self;
}
