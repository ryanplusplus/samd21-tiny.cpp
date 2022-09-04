/*!
 * @file
 * @brief
 */

#ifndef SercomBufferedUartBase_hpp
#define SercomBufferedUartBase_hpp

extern "C" {
#include "sam.h"
}

#include <cstring>
#include "Clock.hpp"
#include "Dma.hpp"
#include "tiny/hal/IBufferedUart.hpp"
#include "tiny/SingleSubscriberEvent.hpp"

template <uint32_t pm_mask, uint8_t gclk_id, Sercom* sercom(), void configure_pins(), uint32_t txpo, uint32_t rxpo, uint32_t dmac_sercom_tx_id, uint32_t dmac_sercom_rx_id>
class SercomBufferedUartBase : public tiny::IBufferedUart {
 public:
  void init(uint32_t baud)
  {
    initialize_peripheral(baud);
    configure_receive_channel();
    configure_send_channel();
  }

 public:
  void send(const void* buffer, uint16_t buffer_size) override
  {
    auto& d = Dma::descriptor(send_channel);
    d.BTCNT.bit.BTCNT = buffer_size;
    d.SRCADDR.bit.SRCADDR = (uintptr_t)buffer + buffer_size;
    d.DSTADDR.bit.DSTADDR = (uintptr_t)&sercom()->USART.DATA.reg;

    Dma::enable(
      send_channel,
      DMAC_CHCTRLB_TRIGACT_BEAT_Val,
      dmac_sercom_tx_id,
      DMAC_CHCTRLB_LVL_LVL0_Val);
  }

  tiny::IEvent<>& on_send_complete() override
  {
    return send_complete;
  }

  tiny::IEvent<const void*, uint16_t>& on_receive() override
  {
    return receive;
  }

  void run() override
  {
    handle_receive_ready();
    handle_send_complete();
  }

 private:
  void configure_receive_channel(void)
  {
    receive_channel = Dma::claim();

    auto& d = Dma::descriptor(receive_channel);
    d.BTCTRL.bit.STEPSIZE = DMAC_BTCTRL_STEPSIZE_X1_Val;
    d.BTCTRL.bit.STEPSEL = DMAC_BTCTRL_STEPSEL_DST_Val;
    d.BTCTRL.bit.DSTINC = 1;
    d.BTCTRL.bit.SRCINC = 0;
    d.BTCTRL.bit.BEATSIZE = DMAC_BTCTRL_BEATSIZE_BYTE_Val;
    d.BTCTRL.bit.BLOCKACT = DMAC_BTCTRL_BLOCKACT_NOACT_Val;
    d.BTCTRL.bit.VALID = 1;
    d.BTCNT.bit.BTCNT = receive_buffer_size;
    d.SRCADDR.bit.SRCADDR = reinterpret_cast<uintptr_t>(&sercom()->USART.DATA.reg);
    d.DSTADDR.bit.DSTADDR = reinterpret_cast<uintptr_t>(receive_buffer + receive_buffer_size);
    d.DESCADDR.bit.DESCADDR = reinterpret_cast<uintptr_t>(&d);

    Dma::enable(
      receive_channel,
      DMAC_CHCTRLB_TRIGACT_BEAT_Val,
      dmac_sercom_rx_id,
      DMAC_CHCTRLB_LVL_LVL3_Val);
  }

  void configure_send_channel(void)
  {
    send_channel = Dma::claim();

    auto& d = Dma::descriptor(send_channel);
    d.BTCTRL.bit.STEPSIZE = DMAC_BTCTRL_STEPSIZE_X1_Val;
    d.BTCTRL.bit.STEPSEL = DMAC_BTCTRL_STEPSEL_SRC_Val;
    d.BTCTRL.bit.DSTINC = 0;
    d.BTCTRL.bit.SRCINC = 1;
    d.BTCTRL.bit.BEATSIZE = DMAC_BTCTRL_BEATSIZE_BYTE_Val;
    d.BTCTRL.bit.BLOCKACT = DMAC_BTCTRL_BLOCKACT_NOACT_Val;
    d.BTCTRL.bit.VALID = 1;
    d.DESCADDR.bit.DESCADDR = 0;

    Dma::install_interrupt_handler(
      send_channel, this, +[](decltype(this) _this) {
        _this->send_completed = true;
      });
    Dma::enable_interrupt(send_channel);
  }

  void initialize_peripheral(uint32_t baud)
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

    sercom()->USART.CTRLA.reg =
      SERCOM_USART_CTRLA_MODE_USART_INT_CLK | // Internal clock
      txpo |
      rxpo |
      SERCOM_USART_CTRLA_DORD; // LSB first

    sercom()->USART.CTRLB.reg =
      SERCOM_USART_CTRLB_TXEN |
      SERCOM_USART_CTRLB_RXEN;
    while(sercom()->USART.SYNCBUSY.bit.CTRLB) {
    }

    sercom()->USART.BAUD.reg = 0xFFFF - ((16ULL * 0xFFFF * baud) / Clock::gclk0_frequency);

    sercom()->USART.CTRLA.bit.ENABLE = 1;
    while(sercom()->USART.SYNCBUSY.bit.ENABLE) {
    }
  }

  void handle_send_complete()
  {
    if(send_completed) {
      send_completed = false;
      send_complete.publish();
    }
  }

  void handle_receive_ready()
  {
    uint16_t receive_head = _receive_head();

    if(receive_head != receive_tail) {
      uint8_t publication_buffer[receive_buffer_size];
      uint16_t count;

      if(receive_tail > receive_head) {
        memcpy(publication_buffer, receive_buffer + receive_tail, receive_buffer_size - receive_tail);
        memcpy(publication_buffer + (receive_buffer_size - receive_tail), receive_buffer, receive_head);
        count = receive_buffer_size - receive_tail + receive_head;
      }
      else {
        memcpy(publication_buffer, receive_buffer + receive_tail, receive_head - receive_tail);
        count = receive_head - receive_tail;
      }

      receive_tail = receive_head;

      receive.publish(publication_buffer, count);
    }
  }

  uint16_t _receive_head()
  {
    auto channel = DMAC->ACTIVE.bit.ID;
    auto beat_count = DMAC->ACTIVE.bit.BTCNT;
    uint16_t receive_dma_beat_count;

    if(channel == static_cast<uint8_t>(receive_channel)) {
      receive_dma_beat_count = beat_count;
    }
    else {
      receive_dma_beat_count = Dma::write_back_descriptor(receive_channel).BTCNT.reg;
    }

    return receive_buffer_size - receive_dma_beat_count;
  }

 private:
  enum {
    receive_buffer_size = 100
  };

  Dma::Channel send_channel{};
  Dma::Channel receive_channel{};

  uint8_t receive_buffer[receive_buffer_size]{};
  uint16_t receive_tail{};

  bool send_completed{};

  tiny::SingleSubscriberEvent<> send_complete{};
  tiny::SingleSubscriberEvent<const void*, uint16_t> receive{};
};

#endif
