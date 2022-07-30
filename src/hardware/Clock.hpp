/*!
 * @file
 * @brief
 */

#ifndef Clock_hpp
#define Clock_hpp

class Clock {
 public:
  enum {
    gclk0_frequency = 48000000,
    gclk2_frequency = 32768,

    gclk_main_frequency = gclk0_frequency
  };

 public:
  static void init();
};

#endif
