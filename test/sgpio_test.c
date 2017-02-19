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
  int retv;
  //int gpio_num = 12; // 12 - Orange Pi Zero GPIO on pin 3 (SDA.0)
  int gpio_num = 6;  // 6 - Orange Pi Zero GPIO on pin 7 (GPIO.7)

  sgpio_init(&gpio, gpio_num);

  if (1) // export
  {
    retv = sgpio_export(gpio_num);
    printf(">>> sgpio_export(%d): '%s'\n",
           gpio_num, sgpio_error_str(retv));
  }

  if (1) // input test
  {

/*
    retv = sgpio_set_edge(&gpio, SGPIO_EDGE_BOTH);
    printf(">>> sgpio_set_edge(%d,%d): '%s'\n",
           sgpio_get_num(&gpio), SGPIO_EDGE_BOTH, sgpio_error_str(retv));
    
    retv = sgpio_set_edge(&gpio, SGPIO_EDGE_RISING);
    printf(">>> sgpio_set_edge(%d,%d): '%s'\n",
           sgpio_get_num(&gpio), SGPIO_EDGE_RISING, sgpio_error_str(retv));
*/  

    retv = sgpio_set_edge(&gpio, SGPIO_EDGE_FALLING);
    printf(">>> sgpio_set_edge(%d,%d): '%s'\n",
           sgpio_get_num(&gpio), SGPIO_EDGE_FALLING, sgpio_error_str(retv));

    retv = sgpio_set_dir(&gpio, SGPIO_DIR_IN);
    printf(">>> sgpio_set_dir(%d,%d): '%s'\n",
           sgpio_get_num(&gpio), SGPIO_DIR_IN, sgpio_error_str(retv));
    
    retv = sgpio_get_val(&gpio);
    printf(">>> sgpio_get_val(%i) = %d",
           sgpio_get_num(&gpio), retv);
    if (retv < 0)
      printf(" '%s'\n", sgpio_error_str(retv));
    else
      printf("\n");

#if 0
    retv = sgpio_poll(&gpio, 10000);
    printf(">>> sgpio_poll(%i) = %d",
           sgpio_get_num(&gpio), retv);
    if (retv < 0)
      printf(" '%s'\n", sgpio_error_str(retv));
    else
      printf("\n");
#else
    retv = sgpio_epoll(&gpio, 10000);
    printf(">>> sgpio_epoll(%i) = %d",
           sgpio_get_num(&gpio), retv);
    if (retv < 0)
      printf(" '%s'\n", sgpio_error_str(retv));
    else
      printf("\n");
#endif

    retv = sgpio_get_val(&gpio);
    printf(">>> sgpio_get_val(%i) = %d",
           sgpio_get_num(&gpio), retv);
    if (retv < 0)
      printf(" '%s'\n", sgpio_error_str(retv));
    else
      printf("\n");

  }
  
  if (0) // output test
  {
    retv = sgpio_set_dir(&gpio, SGPIO_DIR_OUT);
    printf(">>> sgpio_set_dir(%d,%d): '%s'\n",
           sgpio_get_num(&gpio), SGPIO_DIR_OUT,
           sgpio_error_str(retv));

    retv = sgpio_set_val(&gpio, 1);
    printf(">>> sgpio_set_val(%i,1): '%s'\n",
           sgpio_get_num(&gpio),
           sgpio_error_str(retv));
    
    retv = sgpio_set_val(&gpio, 0);
    printf(">>> sgpio_set_val(%i,0): '%s'\n",
           sgpio_get_num(&gpio),
           sgpio_error_str(retv));

#if 0
    while (1)
    {
      sgpio_set_val(&gpio, 0);
      sgpio_set_val(&gpio, 1);
    }
#endif
  }


  if (1) // set to input (more safe mode)
  {
    retv = sgpio_set_dir(&gpio, SGPIO_DIR_IN);
    printf(">>> sgpio_set_dir(%d,%d): '%s'\n",
           sgpio_get_num(&gpio), SGPIO_DIR_IN,
           sgpio_error_str(retv));
  }

  if (1) // unexport
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

