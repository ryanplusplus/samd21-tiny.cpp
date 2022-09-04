/*!
 * @file
 * @brief
 */

#ifndef Sercom2Pa09Pa10Pa11Spi_hpp
#define Sercom2Pa09Pa10Pa11Spi_hpp

#include <stdint.h>
#include <stdbool.h>
#include "tiny/hal/ISpi.hpp"

class Sercom2Pa09Pa10Pa11Spi {
 public:
  static tiny::ISpi& get_instance(
    bool cpol,
    bool cpha,
    bool msb_first,
    uint32_t baud);
};

#endif
