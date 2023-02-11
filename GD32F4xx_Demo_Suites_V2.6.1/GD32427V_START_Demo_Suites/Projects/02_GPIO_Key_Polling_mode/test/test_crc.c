#include "crc32.h"
#include "littleshell.h"
#include <stdio.h>
#include "gd32f4xx_crc.h"

//#define FAST_CRC32
#define SLOW_CRC32

#define CRC_INITIALVALUE 0xFFFFFFFF

extern unsigned long __checksum;

extern unsigned long __ICFEDIT_region_ROM_start__;
extern unsigned long __ICFEDIT_region_ROM_end_CRC__;

#ifdef SLOW_CRC32
static const unsigned long zero=0;
#endif

unsigned int crc(char argc,char ** argv)
{
    unsigned long sum=0;
 #ifdef FAST_CRC32
     sum = set_crc32_init_value (CRC_INITIALVALUE);
     sum = fast_crc32(sum, (unsigned char*)&__ICFEDIT_region_ROM_start__,
                      (unsigned long)&__ICFEDIT_region_ROM_end_CRC__ - (unsigned long)&__ICFEDIT_region_ROM_start__ + 1);
#elif defined SLOW_CRC32
     sum = slow_crc32(CRC_INITIALVALUE, (unsigned char*)&__ICFEDIT_region_ROM_start__,
                      (unsigned long)&__ICFEDIT_region_ROM_end_CRC__  - (unsigned long)&__ICFEDIT_region_ROM_start__ + 1);
     sum = slow_crc32(sum, (unsigned char *)&zero, 4);
#endif
     if(sum!=__checksum)
     {
          printf("Checksum failed: 0x%04lX (calculated) "
                 "vs 0x%04lX (from linker)\n", sum, __checksum);
     }
     else
     {
          printf("* Checksum 0x%04lX is correct! *\n", __checksum);
     }
    return 1;
}
LTSH_FUNCTION_EXPORT(crc,"test crc");

unsigned int crchard(char argc,char ** argv)
{
     unsigned long sum=0;
#if 0
     crc_deinit();

     sum = crc_block_data_calculate((uint32_t *)&__ICFEDIT_region_ROM_start__,
                              ((unsigned long)&__ICFEDIT_region_ROM_end_CRC__ - (unsigned long)&__ICFEDIT_region_ROM_start__ + 1)/4);
     if(sum!=__checksum)
     {
          printf("Checksum failed: 0x%04lX (calculated) "
                 "vs 0x%04lX (from linker)\n", sum, __checksum);
     }
     else
     {
          printf("* Checksum 0x%04lX is correct! *\n", __checksum);
     }
#endif
     uint32_t test[10] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09};
     //crc_deinit();
     crc_data_register_reset();
     sum = crc_block_data_calculate(test,1);
     printf("%x \r\n",sum);

     sum = set_crc32_init_value (CRC_INITIALVALUE);
     sum = fast_crc32(sum,(unsigned char*)test,4);
     printf("%x \r\n",sum);
    return 1;
}
LTSH_FUNCTION_EXPORT(crchard,"test crc with hard");

unsigned int crcsymbol(char argc,char ** argv)
{
    printf("checksum start %x  end %x checksum %x\r\n",__ICFEDIT_region_ROM_start__,__ICFEDIT_region_ROM_end_CRC__,__checksum);
    printf("checksum start %x  end %x checksum %x\r\n",&__ICFEDIT_region_ROM_start__,&__ICFEDIT_region_ROM_end_CRC__,&__checksum);
    return 1;
}
LTSH_FUNCTION_EXPORT(crcsymbol,"test crc symbol");