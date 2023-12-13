/*!
 * @file
 * @brief
 */

#ifndef SercomI2cBase_hpp
#define SercomI2cBase_hpp

extern "C" {
#include "sam.h"
}

#include "Clock.hpp"
#include "tiny/hal/II2c.hpp"

template <uint32_t pm_mask, uint8_t gclk_id, Sercom* sercom(), void configure_pins()>
class SercomI2cBase : public tiny::II2c {
 public:
  void init()
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

    sercom()->I2CM.CTRLA.reg =
      SERCOM_I2CM_CTRLA_MODE_I2C_MASTER |
      SERCOM_I2CM_CTRLA_MEXTTOEN |
      SERCOM_I2CM_CTRLA_SEXTTOEN |
      SERCOM_I2CM_CTRLA_INACTOUT(3) | // 20-21 cycle timeout
      SERCOM_I2CM_CTRLA_LOWTOUTEN;

    sercom()->I2CM.CTRLB.reg =
      SERCOM_I2CM_CTRLB_SMEN;

    sercom()->I2CM.BAUD.reg = Clock::gclk0_frequency / ((2 * standard_mode_hz) - 5);

    sercom()->I2CM.CTRLA.bit.ENABLE = 1;
    while(sercom()->I2CM.SYNCBUSY.bit.ENABLE) {
    }

    // Go to idle bus state
    sercom()->I2CM.STATUS.bit.BUSSTATE = 1;
    while(sercom()->I2CM.SYNCBUSY.reg) {
    }
  }

 public:
  bool write(
    uint8_t address,
    bool prepare_for_restart,
    const uint8_t* _buffer,
    uint16_t buffer_size) override
  {
    auto buffer = reinterpret_cast<const uint8_t*>(_buffer);

    wait_for_operation_to_complete();

    sercom()->I2CM.ADDR.reg = (address << 1) | write_command;

    if(!wait_for_mb()) {
      goto write_error;
    }

    if(client_nacked()) {
      goto write_error;
    }

    for(uint16_t i = 0; i < buffer_size; i++) {
      wait_for_operation_to_complete();

      sercom()->I2CM.DATA.reg = buffer[i];

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
    reset();
    return false;
  }

  bool read(
    uint8_t address,
    bool prepare_for_restart,
    uint8_t* _buffer,
    uint16_t buffer_size) override
  {
    auto buffer = reinterpret_cast<uint8_t*>(_buffer);

    if(!buffer_size) {
      goto read_error;
    }

    wait_for_operation_to_complete();

    sercom()->I2CM.ADDR.reg = (address << 1) | read_command;

    if(!wait_for_sb()) {
      goto read_error;
    }

    if(client_nacked()) {
      goto read_error;
    }

    prepare_to_send_ack();

    for(uint16_t i = 0; i < buffer_size - 1; i++) {
      wait_for_operation_to_complete();

      buffer[i] = sercom()->I2CM.DATA.reg;

      if(!wait_for_sb()) {
        goto read_error;
      }
    }

    prepare_to_send_nack();

    wait_for_operation_to_complete();

    buffer[buffer_size - 1] = sercom()->I2CM.DATA.reg;

    if(!prepare_for_restart) {
      send_stop_condition();
    }

    return true;

  read_error:
    reset();
    return false;
  }

  void reset() override
  {
    sercom()->I2CM.CTRLA.bit.SWRST = 1;
    while(sercom()->I2CM.SYNCBUSY.bit.SWRST) {
    }

    init();
  }

 private:
  static inline bool wait_for_mb()
  {
    volatile uint32_t timeout = 100000;
    while(!sercom()->I2CM.INTFLAG.bit.MB && !sercom()->I2CM.INTFLAG.bit.ERROR && --timeout) {
    }

    return !sercom()->I2CM.INTFLAG.bit.ERROR && sercom()->I2CM.INTFLAG.bit.MB;
  }

  static inline bool wait_for_sb()
  {
    while(!sercom()->I2CM.INTFLAG.bit.SB && !sercom()->I2CM.INTFLAG.bit.ERROR) {
    }

    return !sercom()->I2CM.INTFLAG.bit.ERROR;
  }

  static inline void wait_for_operation_to_complete()
  {
    while(sercom()->I2CM.SYNCBUSY.bit.SYSOP) {
    }
  }

  static inline void prepare_to_send_ack()
  {
    sercom()->I2CM.CTRLB.bit.ACKACT = send_ack;
  }

  static inline void prepare_to_send_nack()
  {
    sercom()->I2CM.CTRLB.bit.ACKACT = send_nack;
  }

  static inline bool client_nacked()
  {
    return sercom()->I2CM.STATUS.bit.RXNACK;
  }

  static inline void send_stop_condition()
  {
    sercom()->I2CM.CTRLB.bit.CMD = 3;
  }

 private:
  enum {
    standard_mode_hz = 100000,

    read_command = 1,
    write_command = 0,

    send_ack = 0,
    send_nack = 1
  };
};

#endif
