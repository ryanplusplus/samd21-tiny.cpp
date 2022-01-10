/*!
 * @file
 * @brief
 */

#ifndef async_spi_sercom4_pa12_pb10_pb11_h
#define async_spi_sercom4_pa12_pb10_pb11_h

#include <stdint.h>
#include <stdbool.h>
#include "hal/i_tiny_async_spi.h"

i_tiny_async_spi_t* async_spi_sercom4_pa12_pb10_pb11_init(bool cpol, bool cpha, bool msb_first, uint32_t baud);

#endif
