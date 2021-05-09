/*!
 * @file
 * @brief
 */

#include <stddef.h>
#include "sam.h"
#include "clock.h"
#include "i2c_sercom3_pa22_pa23.h"

enum {
  standard_mode_hz = 100000,

  read_command = 1,
  write_command = 0,

  send_ack = 0,
  send_nack = 1
};

static void reset(i_tiny_i2c_t* self);

static inline bool wait_for_mb(void)
{
  while(!SERCOM3->I2CM.INTFLAG.bit.MB && !SERCOM3->I2CM.INTFLAG.bit.ERROR) {
  }

  return !SERCOM3->I2CM.INTFLAG.bit.ERROR;
}

static inline bool wait_for_sb(void)
{
  while(!SERCOM3->I2CM.INTFLAG.bit.SB && !SERCOM3->I2CM.INTFLAG.bit.ERROR) {
  }

  return !SERCOM3->I2CM.INTFLAG.bit.ERROR;
}

static inline void wait_for_operation_to_complete(void)
{
  while(SERCOM3->I2CM.SYNCBUSY.bit.SYSOP) {
  }
}

static inline void prepare_to_send_ack(void)
{
  SERCOM3->I2CM.CTRLB.bit.ACKACT = send_ack;
}

static inline void prepare_to_send_nack(void)
{
  SERCOM3->I2CM.CTRLB.bit.ACKACT = send_nack;
}

static inline bool client_nacked(void)
{
  return SERCOM3->I2CM.STATUS.bit.RXNACK;
}

static inline void send_stop_condition(void)
{
  SERCOM3->I2CM.CTRLB.bit.CMD = 3;
}

static bool write(
  i_tiny_i2c_t* self,
  uint8_t address,
  bool prepare_for_restart,
  const uint8_t* buffer,
  uint16_t buffer_size)
{
  (void)self;

  wait_for_operation_to_complete();

  SERCOM3->I2CM.ADDR.reg = (address << 1) | write_command;

  if(!wait_for_mb()) {
    goto write_error;
  }

  if(client_nacked()) {
    goto write_error;
  }

  for(uint16_t i = 0; i < buffer_size - 1; i++) {
    wait_for_operation_to_complete();

    SERCOM3->I2CM.DATA.reg = buffer[i];

    if(!wait_for_mb()) {
      goto write_error;
    }

    if(client_nacked()) {
      goto write_error;
    }
  }

  wait_for_operation_to_complete();

  if(!prepare_for_restart) {
    send_stop_condition();
  }

  return true;

write_error:
  reset(NULL);
  return false;
}

static bool read(
  i_tiny_i2c_t* self,
  uint8_t address,
  bool prepare_for_restart,
  uint8_t* buffer,
  uint16_t buffer_size)
{
  (void)self;

  if(!buffer_size) {
    goto read_error;
  }

  wait_for_operation_to_complete();

  SERCOM3->I2CM.ADDR.reg = (address << 1) | read_command;

  if(!wait_for_sb()) {
    goto read_error;
  }

  if(client_nacked()) {
    goto read_error;
  }

  prepare_to_send_ack();

  for(uint16_t i = 0; i < buffer_size - 1; i++) {
    wait_for_operation_to_complete();

    buffer[i] = SERCOM3->I2CM.DATA.reg;

    if(!wait_for_sb()) {
      goto read_error;
    }
  }

  prepare_to_send_nack();

  wait_for_operation_to_complete();

  buffer[buffer_size - 1] = SERCOM3->I2CM.DATA.reg;

  if(!prepare_for_restart) {
    send_stop_condition();
  }

  return true;

read_error:
  reset(NULL);
  return false;
}

static inline void initialize_peripheral()
{
  // Enable SERCOM3 clock
  PM->APBCMASK.bit.SERCOM3_ = 1;

  // Select GCLK0 (DFLL48)
  GCLK->CLKCTRL.reg =
    GCLK_CLKCTRL_CLKEN |
    GCLK_CLKCTRL_GEN_GCLK0 |
    GCLK_CLKCTRL_ID(SERCOM3_GCLK_ID_CORE);
  while(GCLK->STATUS.bit.SYNCBUSY) {
  }

  PORT->Group[0].PMUX[22 >> 1].bit.PMUXE = MUX_PA22C_SERCOM3_PAD0;
  PORT->Group[0].PINCFG[22].bit.PMUXEN = 1;

  PORT->Group[0].PMUX[23 >> 1].bit.PMUXO = MUX_PA23C_SERCOM3_PAD1;
  PORT->Group[0].PINCFG[23].bit.PMUXEN = 1;

  SERCOM3->I2CM.CTRLA.reg =
    SERCOM_I2CM_CTRLA_MODE_I2C_MASTER |
    SERCOM_I2CM_CTRLA_MEXTTOEN |
    SERCOM_I2CM_CTRLA_SEXTTOEN |
    SERCOM_I2CM_CTRLA_INACTOUT(3) | // 20-21 cycle timeout
    SERCOM_I2CM_CTRLA_LOWTOUTEN;

  SERCOM3->I2CM.CTRLB.reg =
    SERCOM_I2CM_CTRLB_SMEN;

  SERCOM3->I2CM.BAUD.reg = clock_gclk0_frequency / ((2 * standard_mode_hz) - 5);

  SERCOM3->I2CM.CTRLA.bit.ENABLE = 1;
  while(SERCOM3->I2CM.SYNCBUSY.bit.ENABLE) {
  }

  // Go to idle bus state
  SERCOM3->I2CM.STATUS.bit.BUSSTATE = 1;
  while(SERCOM3->I2CM.SYNCBUSY.reg) {
  }
}

static void reset(i_tiny_i2c_t* self)
{
  (void)self;

  SERCOM3->I2CM.CTRLA.bit.SWRST = 1;
  while(SERCOM3->I2CM.SYNCBUSY.bit.SWRST) {
  }

  initialize_peripheral();
}

static const i_tiny_i2c_api_t api = { write, read, reset };

i_tiny_i2c_t* i2c_sercom3_pa22_pa23_init(void)
{
  initialize_peripheral();

  static i_tiny_i2c_t self;
  self.api = &api;
  return &self;
}
