/*!
 * @file
 * @brief
 */

#ifndef Sercom4Pa12Pb10Pb11AsyncSpi_hpp
#define Sercom4Pa12Pb10Pb11AsyncSpi_hpp

#include <stdbool.h>
#include <stdint.h>
#include "tiny/hal/IAsyncSpi.hpp"

class Sercom4Pa12Pb10Pb11AsyncSpi {
 public:
  static tiny::IAsyncSpi& get_instance(
    bool cpol,
    bool cpha,
    bool msb_first,
    uint32_t baud);
};

#endif
