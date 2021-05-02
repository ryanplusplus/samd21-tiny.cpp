/*!
 * @file
 * @brief
 */

#ifndef watchdog_h
#define watchdog_h

#include "wdt.h"

#include "tiny_timer.h"

void watchdog_init(tiny_timer_group_t* timer_group);

#endif
