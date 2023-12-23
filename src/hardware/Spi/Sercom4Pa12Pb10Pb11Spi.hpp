/*!
 * @file
 * @brief
 */

#ifndef Sercom4Pa12Pb10Pb11Spi_hpp
#define Sercom4Pa12Pb10Pb11Spi_hpp

#include <stdbool.h>
#include <stdint.h>
#include "tiny/hal/ISpi.hpp"

class Sercom4Pa12Pb10Pb11Spi {
 public:
  static tiny::ISpi& get_instance(
    bool cpol,
    bool cpha,
    bool msb_first,
    uint32_t baud);
};

#endif
