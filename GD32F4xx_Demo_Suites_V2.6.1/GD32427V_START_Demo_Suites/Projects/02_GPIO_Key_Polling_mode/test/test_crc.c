#include "crc32.h"
#include "littleshell.h"
#include <stdio.h>

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

unsigned int crcsymbol(char argc,char ** argv)
{
    printf("checksum start %x  end %x checksum %x\r\n",__ICFEDIT_region_ROM_start__,__ICFEDIT_region_ROM_end_CRC__,__checksum);
    printf("checksum start %x  end %x checksum %x\r\n",&__ICFEDIT_region_ROM_start__,&__ICFEDIT_region_ROM_end_CRC__,&__checksum);
    return 1;
}
LTSH_FUNCTION_EXPORT(crcsymbol,"test crc symbol");