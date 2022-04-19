/*!
 * @file
 * @brief
 */

#include <stddef.h>
#include "sam.h"
#include "clock.h"
#include "i2c_sercom2_pa08_pa09.h"
#include "tiny_utils.h"

#ifdef SERCOM2

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
  while(!SERCOM2->I2CM.INTFLAG.bit.MB && !SERCOM2->I2CM.INTFLAG.bit.ERROR) {
  }

  return !SERCOM2->I2CM.INTFLAG.bit.ERROR;
}

static inline bool wait_for_sb(void)
{
  while(!SERCOM2->I2CM.INTFLAG.bit.SB && !SERCOM2->I2CM.INTFLAG.bit.ERROR) {
  }

  return !SERCOM2->I2CM.INTFLAG.bit.ERROR;
}

static inline void wait_for_operation_to_complete(void)
{
  while(SERCOM2->I2CM.SYNCBUSY.bit.SYSOP) {
  }
}

static inline void prepare_to_send_ack(void)
{
  SERCOM2->I2CM.CTRLB.bit.ACKACT = send_ack;
}

static inline void prepare_to_send_nack(void)
{
  SERCOM2->I2CM.CTRLB.bit.ACKACT = send_nack;
}

static inline bool client_nacked(void)
{
  return SERCOM2->I2CM.STATUS.bit.RXNACK;
}

static inline void send_stop_condition(void)
{
  SERCOM2->I2CM.CTRLB.bit.CMD = 3;
}

static bool write(
  i_tiny_i2c_t* self,
  uint8_t address,
  bool prepare_for_restart,
  const void* _buffer,
  uint16_t buffer_size)
{
  (void)self;

  reinterpret(buffer, _buffer, const uint8_t*);

  wait_for_operation_to_complete();

  SERCOM2->I2CM.ADDR.reg = (address << 1) | write_command;

  if(!wait_for_mb()) {
    goto write_error;
  }

  if(client_nacked()) {
    goto write_error;
  }

  for(uint16_t i = 0; i < buffer_size - 1; i++) {
    wait_for_operation_to_complete();

    SERCOM2->I2CM.DATA.reg = buffer[i];

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
  void* _buffer,
  uint16_t buffer_size)
{
  (void)self;

  reinterpret(buffer, _buffer, uint8_t*);

  if(!buffer_size) {
    goto read_error;
  }

  wait_for_operation_to_complete();

  SERCOM2->I2CM.ADDR.reg = (address << 1) | read_command;

  if(!wait_for_sb()) {
    goto read_error;
  }

  if(client_nacked()) {
    goto read_error;
  }

  prepare_to_send_ack();

  for(uint16_t i = 0; i < buffer_size - 1; i++) {
    wait_for_operation_to_complete();

    buffer[i] = SERCOM2->I2CM.DATA.reg;

    if(!wait_for_sb()) {
      goto read_error;
    }
  }

  prepare_to_send_nack();

  wait_for_operation_to_complete();

  buffer[buffer_size - 1] = SERCOM2->I2CM.DATA.reg;

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
  // Enable SERCOM2 clock
  PM->APBCMASK.bit.SERCOM2_ = 1;

  // Select GCLK0 (DFLL48)
  GCLK->CLKCTRL.reg =
    GCLK_CLKCTRL_CLKEN |
    GCLK_CLKCTRL_GEN_GCLK0 |
    GCLK_CLKCTRL_ID(SERCOM2_GCLK_ID_CORE);
  while(GCLK->STATUS.bit.SYNCBUSY) {
  }

  PORT->Group[0].PMUX[8 >> 1].bit.PMUXE = MUX_PA08D_SERCOM2_PAD0;
  PORT->Group[0].PINCFG[8].bit.PMUXEN = 1;

  PORT->Group[0].PMUX[9 >> 1].bit.PMUXO = MUX_PA09D_SERCOM2_PAD1;
  PORT->Group[0].PINCFG[9].bit.PMUXEN = 1;

  SERCOM2->I2CM.CTRLA.reg =
    SERCOM_I2CM_CTRLA_MODE_I2C_MASTER |
    SERCOM_I2CM_CTRLA_MEXTTOEN |
    SERCOM_I2CM_CTRLA_SEXTTOEN |
    SERCOM_I2CM_CTRLA_INACTOUT(3) | // 20-21 cycle timeout
    SERCOM_I2CM_CTRLA_LOWTOUTEN;

  SERCOM2->I2CM.CTRLB.reg =
    SERCOM_I2CM_CTRLB_SMEN;

  SERCOM2->I2CM.BAUD.reg = clock_gclk0_frequency / ((2 * standard_mode_hz) - 5);

  SERCOM2->I2CM.CTRLA.bit.ENABLE = 1;
  while(SERCOM2->I2CM.SYNCBUSY.bit.ENABLE) {
  }

  // Go to idle bus state
  SERCOM2->I2CM.STATUS.bit.BUSSTATE = 1;
  while(SERCOM2->I2CM.SYNCBUSY.reg) {
  }
}

static void reset(i_tiny_i2c_t* self)
{
  (void)self;

  SERCOM2->I2CM.CTRLA.bit.SWRST = 1;
  while(SERCOM2->I2CM.SYNCBUSY.bit.SWRST) {
  }

  initialize_peripheral();
}

static const i_tiny_i2c_api_t api = { write, read, reset };

i_tiny_i2c_t* i2c_sercom2_pa08_pa09_init(void)
{
  initialize_peripheral();

  static i_tiny_i2c_t self;
  self.api = &api;
  return &self;
}

#endif
