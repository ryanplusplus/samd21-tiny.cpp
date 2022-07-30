/*!
 * @file
 * @brief
 */

#ifndef SystemTick_hpp
#define SystemTick_hpp

#include "tiny/ITimeSource.hpp"

class SystemTick {
 public:
  static tiny::ITimeSource& get_instance();
};

#endif
