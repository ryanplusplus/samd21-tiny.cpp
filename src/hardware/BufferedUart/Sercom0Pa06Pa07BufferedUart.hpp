/*!
 * @file
 * @brief
 */

#ifndef Sercom0Pa06Pa07BufferedUart_hpp
#define Sercom0Pa06Pa07BufferedUart_hpp

#include "tiny/hal/IBufferedUart.hpp"

class Sercom0Pa06Pa07BufferedUart {
 public:
  static tiny::IBufferedUart& get_instance(uint32_t baud);
};

#endif
