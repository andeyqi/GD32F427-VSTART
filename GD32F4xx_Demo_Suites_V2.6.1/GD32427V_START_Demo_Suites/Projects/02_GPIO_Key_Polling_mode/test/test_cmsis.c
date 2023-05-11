#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <cmsis_iar.h>
#include "littleshell.h"

unsigned int cmsis(char argc,char ** argv)
{
    int cmd;
    uint32_t u32_tmep1,u32_temp1;

    cmd = atoi(argv[1]);

    switch(cmd)
    {
    case 0:
        u32_tmep1 = __get_CONTROL();
        printf("__get_CONTROL = %x \r\n",u32_tmep1);
        break;
    default:
        break;
    }

    return 0;
}

LTSH_FUNCTION_EXPORT(cmsis,"cmsis test");
