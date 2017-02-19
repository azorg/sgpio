/*
 * Simple GPIO Linux wrappers
 * File: "sgpio.h"
 */

#ifndef SGPIO_H
#define SGPIO_H
//----------------------------------------------------------------------------
// inline macro (platform depended)
#ifndef   SGPIO_INLINE
#  define SGPIO_INLINE static inline
#endif // SGPIO_INLINE
//----------------------------------------------------------------------------
// `sgpio_t` type structure
typedef struct sgpio_ {
  int num; // GPIO number /sys/class/gpio/gpioNUM
  int fd;  // file descriptor of /sys/class/gpio/gpioNUM/value
  //...
} sgpio_t;
//----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
//----------------------------------------------------------------------------
int sgpio_init(sgpio_t *self);
//----------------------------------------------------------------------------
int sgpio_free(sgpio_t *self);
//----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cplusplus
//----------------------------------------------------------------------------
#endif // SGPIO_H

/*** end of "sgpio.h" file ***/

