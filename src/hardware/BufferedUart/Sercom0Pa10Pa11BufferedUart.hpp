/*!
 * @file
 * @brief
 */

#ifndef Sercom0Pa10Pa11BufferedUart_hpp
#define Sercom0Pa10Pa11BufferedUart_hpp

#include "tiny/hal/IBufferedUart.hpp"

class Sercom0Pa10Pa11BufferedUart {
 public:
  static tiny::IBufferedUart& get_instance(uint32_t baud);
};

#endif
