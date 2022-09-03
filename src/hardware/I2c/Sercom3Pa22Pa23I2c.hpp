/*!
 * @file
 * @brief Standard mode (100 KHz)
 */

#ifndef Sercom3Pa22Pa23I2c_hpp
#define Sercom3Pa22Pa23I2c_hpp

#include "tiny/hal/II2c.hpp"

class Sercom3Pa22Pa23I2c {
 public:
  static tiny::II2c& get_instance();
};

#endif
