/*!
 * @file
 * @brief
 */

#ifndef Sercom2Pa09Pa10Pa11AsyncSpi_hpp
#define Sercom2Pa09Pa10Pa11AsyncSpi_hpp

#include <stdbool.h>
#include <stdint.h>
#include "tiny/hal/IAsyncSpi.hpp"

class Sercom2Pa09Pa10Pa11AsyncSpi {
 public:
  static tiny::IAsyncSpi& get_instance(
    bool cpol,
    bool cpha,
    bool msb_first,
    uint32_t baud);
};

#endif
