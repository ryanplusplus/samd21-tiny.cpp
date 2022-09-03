/*!
 * @file
 * @brief
 */

#ifndef Sercom4Pb08Pb09Uart_hpp
#define Sercom4Pb08Pb09Uart_hpp

#include "tiny/hal/IUart.hpp"

class Sercom4Pb08Pb09Uart {
 public:
  static tiny::IUart& get_instance(uint32_t baud);
};

#endif
