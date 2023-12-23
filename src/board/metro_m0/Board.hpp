/*!
 * @file
 * @brief
 */

#ifndef Board_hpp
#define Board_hpp

#include "Clock.hpp"
#include "HeartbeatTemplate.hpp"
#include "Interrupts.hpp"
#include "Neopixel.hpp"
#include "Sercom0Pa10Pa11BufferedUart.hpp"
#include "Sercom0Pa10Pa11Uart.hpp"
#include "Sercom3Pa22Pa23I2c.hpp"
#include "Sercom4Pa12Pb10Pb11AsyncSpi.hpp"
#include "Sercom4Pa12Pb10Pb11Spi.hpp"
#include "SystemTick.hpp"
#include "Watchdog.hpp"

using Heartbeat = HeartbeatTemplate<PIN_PA17>;
using I2c = Sercom3Pa22Pa23I2c;
using AsyncSpi = Sercom4Pa12Pb10Pb11AsyncSpi;
using Spi = Sercom4Pa12Pb10Pb11Spi;
using BufferedUart = Sercom0Pa10Pa11BufferedUart;
using Uart = Sercom0Pa10Pa11Uart;
using NeopixelBff = Neopixel::Neopixel<Neopixel::ByteOrder::grb, Clock::gclk0_frequency, PIN_PA14>;

#endif
