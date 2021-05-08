/*!
 * @file
 * @brief
 */

#ifndef spi_sercom4_pa12_pb10_pb11_h
#define spi_sercom4_pa12_pb10_pb11_h

#include <stdint.h>
#include <stdbool.h>
#include "hal/i_tiny_spi.h"

i_tiny_spi_t* spi_sercom4_pa12_pb10_pb11_init(bool cpol, bool cpha, bool msb_first, uint32_t baud);

#endif
