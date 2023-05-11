#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <cmsis_iar.h>
#include "littleshell.h"
#include "gd32f4xx.h"
#include <core_cm4.h>

unsigned int cmsis(char argc,char ** argv)
{
    int cmd = 0;
    cmd = atoi(argv[1]);
    uint32_t u32_temp1,u32_temp2;
    uint16_t u16_temp1,u16_temp2;

    switch(cmd)
    {
    case 0:
        u32_temp1 = 0xaabbccdd;
        u32_temp2 = __REV(u32_temp1);
        printf("0x%x __REV = 0x%x.\r\n",u32_temp1,u32_temp2);
        u32_temp1 = 0xaabbccdd;
        u32_temp2 = __REV16(u32_temp1);
        printf("0x%x __REV16 = 0x%x.\r\n",u32_temp1,u32_temp2);
        u16_temp1 = 0xaabb;
        u16_temp2 = (uint16_t)__REVSH((int16_t)u16_temp1);
        printf("0x%x __REVSH = 0x%x.\r\n",u16_temp1,u16_temp2);
        break;
    case 1:
        NVIC_SystemReset();
        break;
    case 2:
        u32_temp1 = __get_CONTROL();
        printf("CONTROL %x CONTROL_SPSEL %d CONTROL_nPRIV %d.\r\n",
                u32_temp1,_FLD2VAL(CONTROL_SPSEL,u32_temp1),_FLD2VAL(CONTROL_nPRIV,u32_temp1));
        break;
    case 3:
        if(_FLD2VAL(CoreDebug_DHCSR_C_DEBUGEN,CoreDebug->DHCSR))
            __BKPT(0);
        break;
    default:
        break;
    }

    return 0;
}

LTSH_FUNCTION_EXPORT(cmsis,"cmsis test");
