/*!
 * @file
 * @brief Needed to use -nostartfiles.
 */

#include <stddef.h>

extern void (*__preinit_array_start[])(void);
extern void (*__preinit_array_end[])(void);
extern void (*__init_array_start[])(void);
extern void (*__init_array_end[])(void);

void __libc_init_array(void)
{
  size_t count;

  count = __preinit_array_end - __preinit_array_start;
  for(size_t i = 0; i < count; i++) {
    __preinit_array_start[i]();
  }

  count = __init_array_end - __init_array_start;
  for(size_t i = 0; i < count; i++) {
    __init_array_start[i]();
  }
}
