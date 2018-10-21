/*
 * Simple GPIO Linux wrappers - test module
 * File: "sgpio_test.c"
 */

//---------------------------------------------------------------------------
#include "sgpio.h"
#include <stdio.h>  // printf()
//---------------------------------------------------------------------------
int main()
{
  sgpio_t gpio;
  int retv, i;
  //int gpio_num = 12; // 12 - Orange Pi Zero GPIO on pin 3 (SDA.0)
  //int gpio_num = 6;  // 6 - Orange Pi Zero GPIO on pin 7 (GPIO.7)
  int gpio_num = 1;  // 1 - Orange Pi Zero GPIO on pin 11 (RxD2)

  sgpio_init(&gpio, gpio_num);

  if (1) // unexport
  {
    retv = sgpio_unexport(gpio_num);
    printf(">>> sgpio_unexport(%d): '%s'\n",
           gpio_num, sgpio_error_str(retv));
  }

  if (1) // export
  {
    retv = sgpio_export(gpio_num);
    printf(">>> sgpio_export(%d): '%s'\n",
           gpio_num, sgpio_error_str(retv));
  }

  if (1) // input test
  {

    retv = sgpio_mode(&gpio, SGPIO_DIR_IN, SGPIO_EDGE_BOTH);
    printf(">>> sgpio_mode(%d,%d,%d): '%s'\n",
           sgpio_num(&gpio), SGPIO_DIR_IN, SGPIO_EDGE_BOTH, sgpio_error_str(retv));

    retv = sgpio_get(&gpio);
    printf(">>> sgpio_get(%i) = %d",
           sgpio_num(&gpio), retv);
    if (retv < 0)
      printf(" '%s'\n", sgpio_error_str(retv));
    else
      printf("\n");

for (i = 0; i < 4; i++) {
#if 0
    retv = sgpio_poll(&gpio, 4000);
    printf(">>> sgpio_poll(%i, 4000) = %d",
           sgpio_num(&gpio), retv);
    if (retv < 0)
      printf(" '%s'\n", sgpio_error_str(retv));
    else
      printf("\n");
#else
    retv = sgpio_epoll(&gpio, 4000);
    printf(">>> sgpio_epoll(%i, 4000) = %d",
           sgpio_num(&gpio), retv);
    if (retv < 0)
      printf(" '%s'\n", sgpio_error_str(retv));
    else
      printf("\n");
#endif

    retv = sgpio_get(&gpio);
    printf(">>> sgpio_get(%i) = %d",
           sgpio_num(&gpio), retv);
    if (retv < 0)
      printf(" '%s'\n", sgpio_error_str(retv));
    else
      printf("\n");

    retv = sgpio_get(&gpio);
    printf(">>> sgpio_get(%i) = %d",
           sgpio_num(&gpio), retv);
    if (retv < 0)
      printf(" '%s'\n", sgpio_error_str(retv));
    else
      printf("\n");

  }
}
  
  if (0) // output test
  {
    retv = sgpio_mode(&gpio, SGPIO_DIR_OUT, SGPIO_EDGE_NONE);
    printf(">>> sgpio_mode(%d,%d,%d): '%s'\n",
           sgpio_num(&gpio), SGPIO_DIR_OUT, SGPIO_EDGE_NONE,
           sgpio_error_str(retv));

    for (i = 0; i < 6; i++)
    {
      retv = sgpio_set(&gpio,i & 1);
      printf(">>> sgpio_set(%i,%i): '%s'\n",
             sgpio_num(&gpio), i & 1,
             sgpio_error_str(retv));

      retv = sgpio_get(&gpio);
      printf(">>> sgpio_get(%i) = %d",
            sgpio_num(&gpio), retv);
      if (retv < 0)
        printf(" '%s'\n", sgpio_error_str(retv));
      else
        printf("\n");

      sleep(1);
    } // for
  }


  if (0) // set to input (more safe mode)
  {
    retv = sgpio_mode(&gpio, SGPIO_DIR_IN, SGPIO_EDGE_NONE);
    printf(">>> sgpio_mode(%d,%d,%d): '%s'\n",
           sgpio_num(&gpio), SGPIO_DIR_IN, SGPIO_EDGE_NONE,
           sgpio_error_str(retv));
  }

  if (0) // unexport
  {
    retv = sgpio_unexport(gpio_num);
    printf(">>> sgpio_unexport(%d): '%s'\n",
           gpio_num, sgpio_error_str(retv));
  }

  sgpio_free(&gpio);

  return 0;
}
//---------------------------------------------------------------------------

/*** end of "sgpio_test.c" ***/

