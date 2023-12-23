/*!
 * @file
 * @brief
 */

#ifndef Sercom0Pa05Pa06Pa07AsyncSpi_hpp
#define Sercom0Pa05Pa06Pa07AsyncSpi_hpp

#include <stdbool.h>
#include <stdint.h>
#include "tiny/hal/IAsyncSpi.hpp"

class Sercom0Pa05Pa06Pa07AsyncSpi {
 public:
  static tiny::IAsyncSpi& get_instance(
    bool cpol,
    bool cpha,
    bool msb_first,
    uint32_t baud);
};

#endif
