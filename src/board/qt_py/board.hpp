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
#include "Sercom0Pa06Pa07Uart.hpp"

using Heartbeat = HeartbeatTemplate<HeartbeatPin::none>;
using Uart = Sercom0Pa06Pa07Uart;

#endif
