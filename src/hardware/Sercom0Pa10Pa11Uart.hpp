/*!
 * @file
 * @brief
 */

#ifndef Sercom0Pa10Pa11Uart_hpp
#define Sercom0Pa10Pa11Uart_hpp

#include "tiny/hal/IUart.hpp"

class Sercom0Pa10Pa11Uart {
 public:
  static tiny::IUart& get_instance(uint32_t baud);
};

#endif
