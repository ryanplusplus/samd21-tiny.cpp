/*!
 * @file
 * @brief
 */

#ifndef Board_hpp
#define Board_hpp

#include "Clock.hpp"
#include "SystemTick.hpp"
#include "Watchdog.hpp"
#include "Interrupts.hpp"
#include "HeartbeatTemplate.hpp"
#include "Sercom1Pa16Pa17I2c.hpp"
#include "Sercom0Pa09Pa10Pa11Spi.hpp"
#include "Sercom0Pa09Pa10Pa11AsyncSpi.hpp"
#include "Sercom0Pa06Pa07Uart.hpp"

using Heartbeat = HeartbeatTemplate<HeartbeatPin::none>;
using I2c = Sercom1Pa16Pa17I2c;
using AsyncSpi = Sercom0Pa09Pa10Pa11AsyncSpi;
using Spi = Sercom0Pa09Pa10Pa11Spi;
using Uart = Sercom0Pa06Pa07Uart;

#endif
