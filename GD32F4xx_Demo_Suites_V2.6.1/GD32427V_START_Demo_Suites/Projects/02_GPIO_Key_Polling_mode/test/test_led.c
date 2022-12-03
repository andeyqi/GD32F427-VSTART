#include "gd32f4xx.h"
#include <stdio.h>
#include <string.h>
#include "littleshell.h"
#include "utilities.h"

unsigned int led(char argc,char ** argv)
{
    if(argc != 2)
        return 0;

    if(strcmp("on",argv[1]) == 0)
    {
        /* turn on LED1 */
        gpio_bit_set(GPIOC, GPIO_PIN_6);
    }
    else if(strcmp("off",argv[1]) == 0)
    {
        gpio_bit_reset(GPIOC, GPIO_PIN_6);
    }
    
    return 1;
}
LTSH_FUNCTION_EXPORT(led,"test led on/off");