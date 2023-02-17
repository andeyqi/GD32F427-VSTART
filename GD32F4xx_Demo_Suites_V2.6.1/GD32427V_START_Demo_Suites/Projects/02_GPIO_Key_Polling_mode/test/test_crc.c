#include "crc32.h"
#include "littleshell.h"
#include <stdio.h>
#include "gd32f4xx_crc.h"
#include "perf_counter.h"

//#define FAST_CRC32
#define HARD_CRC32
//#define SLOW_CRC32

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
     sum = fast_crc32(CRC_INITIALVALUE, (unsigned char*)&__ICFEDIT_region_ROM_start__,
                      (unsigned long)&__ICFEDIT_region_ROM_end_CRC__ - (unsigned long)&__ICFEDIT_region_ROM_start__ + 1);
#elif defined SLOW_CRC32
     sum = slow_crc32(CRC_INITIALVALUE, (unsigned char*)&__ICFEDIT_region_ROM_start__,
                      (unsigned long)&__ICFEDIT_region_ROM_end_CRC__  - (unsigned long)&__ICFEDIT_region_ROM_start__ + 1);
#elif defined HARD_CRC32
     crc_data_register_reset();
     sum = crc_block_data_calculate3((unsigned int*)&__ICFEDIT_region_ROM_start__,
     ((unsigned long)&__ICFEDIT_region_ROM_end_CRC__ - (unsigned long)&__ICFEDIT_region_ROM_start__ + 1)/4);
#endif
     if(sum!=__checksum)
     {
          printf("Checksum failed: 0x%04lX (calculated) "
                 "vs 0x%04lX (from linker)\r\n", sum, __checksum);
     }
     else
     {
          printf("* Checksum 0x%04lX is correct! *\r\n", __checksum);
     }
    return 1;
}
LTSH_FUNCTION_EXPORT(crc,"test crc");

unsigned int crchard(char argc,char ** argv)
{
     unsigned long sum=0;
     int32_t counter;
     uint32_t test[10] = {0x01,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09};

     start_cycle_counter();
     crc_data_register_reset();
     sum = crc_block_data_calculate(test,10);
     counter = stop_cycle_counter();
     printf("crc_block_data_calculate %x %d\r\n",sum,counter);

     start_cycle_counter();
     crc_data_register_reset();
     sum = crc_block_data_calculate1((unsigned char*)test,40);
     counter = stop_cycle_counter();
     printf("crc_block_data_calculate1 %x %d\r\n",sum,counter);

     start_cycle_counter();
     crc_data_register_reset();
     sum = crc_block_data_calculate2(test,10);
     counter = stop_cycle_counter();
     printf("crc_block_data_calculate2 %x %d\r\n",sum,counter);

     start_cycle_counter();
     crc_data_register_reset();
     sum = crc_block_data_calculate3(test,10);
     counter = stop_cycle_counter();
     printf("crc_block_data_calculate3 %x %d\r\n",sum,counter);

     start_cycle_counter();
     sum = crc32_mpeg2_fast(0xffffffff,(unsigned char*)test,40);
     counter = stop_cycle_counter();
     printf("crc32_mpeg2_fast %x %d\r\n",sum,counter);

     start_cycle_counter();
     sum = crc32_mpeg2_fast1(0xffffffff,(unsigned char*)test,40);
     counter = stop_cycle_counter();
     printf("crc32_mpeg2_fast1 %x %d\r\n",sum,counter);

     start_cycle_counter();
     sum = crc32_mpeg2_slow(0xffffffff,(unsigned char*)test,40);
     counter = stop_cycle_counter();
     printf("crc32_mpeg2_slow %x %d\r\n",sum,counter);

     start_cycle_counter();
     sum = crc32_mpeg2_slow1(0xffffffff,(unsigned char*)test,40);
     counter = stop_cycle_counter();
     printf("crc32_mpeg2_slow2 %x %d\r\n",sum,counter);

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