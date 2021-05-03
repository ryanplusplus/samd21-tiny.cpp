/*!
 * @file
 * @brief
 */

#ifndef spi_sercom0_h
#define spi_sercom0_h

#include <stdbool.h>
#include <stdint.h>
#include "hal/i_tiny_spi.h"

i_tiny_spi_t* spi_sercom0_init(
  uint8_t cpol,
  uint8_t cpha,
  bool msb_first,
  uint32_t baud);

#endif
