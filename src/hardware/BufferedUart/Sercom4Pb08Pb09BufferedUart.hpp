/*!
 * @file
 * @brief
 */

#ifndef Sercom4Pb08Pb09BufferedUart_hpp
#define Sercom4Pb08Pb09BufferedUart_hpp

#include "tiny/hal/IBufferedUart.hpp"

class Sercom4Pb08Pb09BufferedUart {
 public:
  static tiny::IBufferedUart& get_instance(uint32_t baud);
};

#endif
