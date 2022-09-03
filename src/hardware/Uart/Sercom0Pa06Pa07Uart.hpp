/*!
 * @file
 * @brief
 */

#ifndef Sercom0Pa06Pa07Uart_hpp
#define Sercom0Pa06Pa07Uart_hpp

#include "tiny/hal/IUart.hpp"

class Sercom0Pa06Pa07Uart {
 public:
  static tiny::IUart& get_instance(uint32_t baud);
};

#endif
