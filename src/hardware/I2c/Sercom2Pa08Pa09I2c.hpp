/*!
 * @file
 * @brief Standard mode (100 KHz)
 */

#ifndef Sercom2Pa08Pa09I2c_hpp
#define Sercom2Pa08Pa09I2c_hpp

#include "tiny/hal/II2c.hpp"

class Sercom2Pa08Pa09I2c {
 public:
  static tiny::II2c& get_instance();
};

#endif
