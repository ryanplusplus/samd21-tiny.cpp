/*!
 * @file
 * @brief
 */

#include "usart.h"
#include "usart_interrupt.h"
#include "uart_sercom4.h"
#include "tiny_event.h"

static struct usart_module usart;
static i_tiny_uart_t self;
static tiny_event_t send_complete;
static tiny_event_t receive;
static uint16_t receivedData;

static void rx_callback(const struct usart_module* const module) {
  (void)module;
  tiny_event_publish(&receive, &(tiny_uart_on_receive_args_t){ .byte = receivedData });
  usart_read_job(&usart, &receivedData);
}

static void tx_callback(const struct usart_module* const module) {
  (void)module;
  tiny_event_publish(&send_complete, NULL);
}

static void send(i_tiny_uart_t* self, uint8_t byte) {
  usart_write_job(&usart, byte);
}

static i_tiny_event_t* on_send_complete(i_tiny_uart_t* self) {
  return &send_complete.interface;
}

static i_tiny_event_t* on_receive(i_tiny_uart_t* self) {
  return &receive.interface;
}

static const i_tiny_uart_api_t api = { send, on_send_complete, on_receive };

i_tiny_uart_t* uart_sercom4_init(uint32_t baud) {
  struct usart_config config = {
    .data_order = USART_DATAORDER_LSB,
    .transfer_mode = USART_TRANSFER_ASYNCHRONOUSLY,
    .parity = USART_PARITY_NONE,
    .stopbits = USART_STOPBITS_1,
    .character_size = USART_CHARACTER_SIZE_8BIT,
    .baudrate = baud,
    .receiver_enable = true,
    .transmitter_enable = true,
    .clock_polarity_inverted = false,
    .use_external_clock = false,
    .ext_clock_freq = 0,
    .mux_setting = USART_RX_1_TX_0_RTS_2_CTS_3,
    .run_in_standby = false,
    .generator_source = GCLK_GENERATOR_0,
    .pinmux_pad0 = PINMUX_PB08D_SERCOM4_PAD0,
    .pinmux_pad1 = PINMUX_PB09D_SERCOM4_PAD1,
    .pinmux_pad2 = PINMUX_UNUSED,
    .pinmux_pad3 = PINMUX_UNUSED,
    .sample_adjustment = USART_SAMPLE_ADJUSTMENT_7_8_9,
    .sample_rate = USART_SAMPLE_RATE_16X_ARITHMETIC,
    .lin_slave_enable = false,
    .immediate_buffer_overflow_notification = false,
    .start_frame_detection_enable = false,
    .encoding_format_enable = false,
    .receive_pulse_length = 19,
    .collision_detection_enable = false
  };

  usart_init(&usart, SERCOM4, &config);
  usart_enable(&usart);

  tiny_event_init(&send_complete);
  tiny_event_init(&receive);

  usart_register_callback(&usart, rx_callback, USART_CALLBACK_BUFFER_RECEIVED);
  usart_enable_callback(&usart, USART_CALLBACK_BUFFER_RECEIVED);

  usart_register_callback(&usart, tx_callback, USART_CALLBACK_BUFFER_TRANSMITTED);
  usart_enable_callback(&usart, USART_CALLBACK_BUFFER_TRANSMITTED);

  usart_read_job(&usart, &receivedData);

  self.api = &api;

  return &self;
}
