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
#include "Sercom0Pa10Pa11Uart.hpp"

using Heartbeat = HeartbeatTemplate<PIN_PA17>;
using Uart = Sercom0Pa10Pa11Uart;

#endif
