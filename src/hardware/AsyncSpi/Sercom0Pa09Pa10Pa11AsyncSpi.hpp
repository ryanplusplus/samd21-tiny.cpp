/*!
 * @file
 * @brief
 */

#ifndef Sercom0Pa09Pa10Pa11AsyncSpi_hpp
#define Sercom0Pa09Pa10Pa11AsyncSpi_hpp

#include <stdint.h>
#include <stdbool.h>
#include "tiny/hal/IAsyncSpi.hpp"

class Sercom0Pa09Pa10Pa11AsyncSpi {
 public:
  static tiny::IAsyncSpi& get_instance(
    bool cpol,
    bool cpha,
    bool msb_first,
    uint32_t baud);
};

#endif
