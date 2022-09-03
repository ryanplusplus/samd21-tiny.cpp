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
#include "Sercom2Pa08Pa09I2c.hpp"
#include "Sercom0Pa05Pa06Pa07AsyncSpi.hpp"
#include "Sercom0Pa05Pa06Pa07Spi.hpp"
#include "Sercom4Pb08Pb09Uart.hpp"

using Heartbeat = HeartbeatTemplate<PIN_PA17>;
using I2c = Sercom2Pa08Pa09I2c;
using AsyncSpi = Sercom0Pa05Pa06Pa07AsyncSpi;
using Spi = Sercom0Pa05Pa06Pa07Spi;
using Uart = Sercom4Pb08Pb09Uart;

#endif
