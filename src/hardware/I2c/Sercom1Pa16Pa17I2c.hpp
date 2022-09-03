/*!
 * @file
 * @brief Standard mode (100 KHz)
 */

#ifndef Sercom1Pa16Pa17I2c_hpp
#define Sercom1Pa16Pa17I2c_hpp

#include "tiny/hal/II2c.hpp"

class Sercom1Pa16Pa17I2c {
 public:
  static tiny::II2c& get_instance();
};

#endif
