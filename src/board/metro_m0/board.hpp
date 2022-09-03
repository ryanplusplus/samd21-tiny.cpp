/*!
 * @file
 * @brief
 */

#ifndef board_hpp
#define board_hpp

#include "Clock.hpp"
#include "SystemTick.hpp"
#include "Watchdog.hpp"
#include "Interrupts.hpp"
#include "HeartbeatTemplate.hpp"
#include "Sercom3Pa22Pa23I2c.hpp"
#include "Sercom4Pa12Pb10Pb11Spi.hpp"
#include "Sercom0Pa10Pa11Uart.hpp"

using Heartbeat = HeartbeatTemplate<PIN_PA17>;
using I2c = Sercom3Pa22Pa23I2c;
using Spi = Sercom4Pa12Pb10Pb11Spi;
using Uart = Sercom0Pa10Pa11Uart;

#endif
