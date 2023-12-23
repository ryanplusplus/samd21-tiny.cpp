/*!
 * @file
 * @brief
 */

#ifndef Board_hpp
#define Board_hpp

#include "Clock.hpp"
#include "HeartbeatTemplate.hpp"
#include "Interrupts.hpp"
#include "Sercom0Pa05Pa06Pa07AsyncSpi.hpp"
#include "Sercom0Pa05Pa06Pa07Spi.hpp"
#include "Sercom2Pa08Pa09I2c.hpp"
#include "Sercom4Pb08Pb09BufferedUart.hpp"
#include "Sercom4Pb08Pb09Uart.hpp"
#include "SystemTick.hpp"
#include "Watchdog.hpp"

using Heartbeat = HeartbeatTemplate<PIN_PA17>;
using I2c = Sercom2Pa08Pa09I2c;
using AsyncSpi = Sercom0Pa05Pa06Pa07AsyncSpi;
using Spi = Sercom0Pa05Pa06Pa07Spi;
using BufferedUart = Sercom4Pb08Pb09BufferedUart;
using Uart = Sercom4Pb08Pb09Uart;

#endif
