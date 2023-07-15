#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifndef GD32F4XX_FMC_H

#define REG32(addr)           (*(volatile uint32_t *)(uint32_t)(addr))
#define FMC_BASE              ((uint32_t)0x40020000U + 0x00003C00U)

#define FMC                        FMC_BASE
/* registers definitions */
#define FMC_WS                     REG32((FMC) + 0x00000000U)     /*!< FMC wait state register */
#define FMC_KEY                    REG32((FMC) + 0x00000004U)     /*!< FMC unlock key register */
#define FMC_OBKEY                  REG32((FMC) + 0x00000008U)     /*!< FMC option byte unlock key register */
#define FMC_STAT                   REG32((FMC) + 0x0000000CU)     /*!< FMC status register */
#define FMC_CTL                    REG32((FMC) + 0x00000010U)     /*!< FMC control register */
#define FMC_OBCTL0                 REG32((FMC) + 0x00000014U)     /*!< FMC option byte control register 0 */
#define FMC_OBCTL1                 REG32((FMC) + 0x00000018U)     /*!< FMC option byte control register 1 */
#define FMC_PECFG                  REG32((FMC) + 0x00000020U)     /*!< FMC page erase configuration register */
#define FMC_PEKEY                  REG32((FMC) + 0x00000024U)     /*!< FMC unlock page erase key register */
#define FMC_WSEN                   REG32((FMC) + 0x000000FCU)     /*!< FMC wait state enable register */
#define FMC_PID                    REG32((FMC) + 0x00000100U)     /*!< FMC product ID register */

#endif

static __no_init uint32_t parallelism;
static __no_init uint8_t blankcheck;

void execUserFlashInit(void)
{
    /* unlock flash */
    FMC_KEY = 0x45670123;
    FMC_KEY = 0xCDEF89AB;
    FMC_WS = 0x00000000;
    FMC_CTL = 0x00000000;
    FMC_STAT = 0x000000F0;
}

#define USE_ARGC_ARGV 1

#if USE_ARGC_ARGV
uint32_t FlashInit(void *base_of_flash, uint32_t image_size,
                   uint32_t link_address, uint32_t flags,
                   int argc, char const *argv[])
#else
uint32_t FlashInit(void *base_of_flash, uint32_t image_size,
                   uint32_t link_address, uint32_t flags)
#endif
{
    uint32_t temp = 0;
    parallelism = 1;
    blankcheck = 0;
#ifdef USE_ARGC_ARGV

  for(int i = 0; i < argc; i++)
  {
    /* Parallelism flag*/
    if(strcmp("--x8", argv[i]) == 0)
    {
      parallelism = 0;
    }
    //else if (strcmp("--x16", argv[i]) == 0)
    //{
    //  parallelism = 1;
    //}
    else if (strcmp("--x32", argv[i]) == 0)
    {
      parallelism = 2;
    }
    else if (strcmp("--x64", argv[i]) == 0)
    {
      parallelism = 3;
    }
    else if (strcmp("--skip_erase", argv[i]) == 0)
    {
      blankcheck = 1;
    }
  }
#endif
    FMC_WS = 0x00000000;
    temp = FMC_CTL;
    temp &= ~(0x3u << 8);
    temp |= (parallelism << 8);
    FMC_CTL = temp;

    return 0;
}

uint32_t FlashWrite(void *block_start,
                    uint32_t offset_into_block,
                    uint32_t count,
                    char const *buffer)
{
    uint8_t * dst = (uint8_t *)((unsigned char *)block_start + offset_into_block);
    uint8_t * src = (uint8_t *)buffer;

    FMC_CTL |= 0x01u;
    FMC_STAT = 0x1d2;

    for ( ; count; count -= 8)
    {

        switch (parallelism)
        {
        case 0:
            while (FMC_STAT & (0x01u<<16));
            *dst++ = *src++;
            while (FMC_STAT & (0x01u<<16));
            *dst++ = *src++;
            while (FMC_STAT & (0x01u<<16));
            *dst++ = *src++;
            while (FMC_STAT & (0x01u<<16));
            *dst++ = *src++;
            while (FMC_STAT & (0x01u<<16));
            *dst++ = *src++;
            while (FMC_STAT & (0x01u<<16));
            *dst++ = *src++;
            while (FMC_STAT & (0x01u<<16));
            *dst++ = *src++;
            while (FMC_STAT & (0x01u<<16));
            *dst++ = *src++;
            break;
        case 1:
            while (FMC_STAT & (0x01u<<16));
            *((uint16_t *)dst) = *((uint16_t *)src);
            src += 2;
            dst += 2;
            while (FMC_STAT & (0x01u<<16));
            *((uint16_t *)dst) = *((uint16_t *)src);
            src += 2;
            dst += 2;
            while (FMC_STAT & (0x01u<<16));
            *((uint16_t *)dst) = *((uint16_t *)src);
            src += 2;
            dst += 2;
            while (FMC_STAT & (0x01u<<16));
            *((uint16_t *)dst) = *((uint16_t *)src);
            src += 2;
            dst += 2;
            break;

        case 2:
            while (FMC_STAT & (0x01u<<16));
            *((uint32_t *)dst) = *((uint32_t *)src);
            src += 4;
            dst += 4;
            while (FMC_STAT & (0x01u<<16));
            *((uint32_t *)dst) = *((uint32_t *)src);
            src += 4;
            dst += 4;
            break;

        default:
            while (FMC_STAT & (0x01u<<16));
            *((uint64_t *)dst) = *((uint64_t *)src);
            src += 8;
            dst += 8;
            break;
    }
  }

    while (FMC_STAT & (0x01u<<16));
    FMC_CTL &= ~0x01;

    if(FMC_STAT & 0x1d2)
        return 1;

    return 0;
}

uint32_t FlashErase(void *block_start,
                    uint32_t block_size)
{
    uint32_t offset = (uint32_t)block_start;
    uint32_t sector_num = 0;


    if(offset >= 0x08000000 && offset <= 0x08003fff)
    {
        //sector_num = 0;
        sector_num = 0x00u << 3;
     }
    else if(offset >= 0x08004000 && offset <= 0x08007fff)
    {
        //sector_num = 1;
        sector_num = 0x01u << 3;
    }
    else if(offset >= 0x08008000 && offset <= 0x0800bfff)
    {
        //sector_num = 2;
        sector_num = 0x02u << 3;
    }
    else if(offset >= 0x0800c000 && offset <= 0x0800ffff)
    {
        //sector_num = 3;
        sector_num = 0x03u << 3;
    }
    else if(offset >= 0x08010000 && offset <= 0x0801ffff)
    {
        //sector_num = 4;
        sector_num = 0x04u << 3;
    }
    else if(offset >= 0x08020000 && offset <= 0x0803ffff)
    {
        //sector_num = 5;
        sector_num = 0x05u << 3;
    }
    else if(offset >= 0x08040000 && offset <= 0x0805ffff)
    {
        //sector_num = 6;
        sector_num = 0x06u << 3;
    }
    else if(offset >= 0x08060000 && offset <= 0x0807ffff)
    {
        //sector_num = 7;
        sector_num = 0x07u << 3;
    }
    else if(offset >= 0x08080000 && offset <= 0x0809ffff)
    {
        //sector_num = 8;
        sector_num = 0x08u << 3;
    }
    else if(offset >= 0x080a0000 && offset <= 0x080bffff)
    {
        //sector_num = 9;
        sector_num = 0x09u << 3;
    }
    else if(offset >= 0x080c0000 && offset <= 0x080dffff)
    {
        //sector_num = 10;
        sector_num = 10u << 3;
    }
    else if(offset >= 0x080e0000 && offset <= 0x080fffff)
    {
        //sector_num = 11;
        sector_num = 11u << 3;
    }
    else if(offset >= 0x08100000 && offset <= 0x08103fff)
    {
        //sector_num = 12;
        sector_num = 0x10u << 3;
    }
    else if(offset >= 0x08104000 && offset <= 0x08107fff)
    {
        //sector_num = 13;
        sector_num = 0x11u << 3;
    }
    else if(offset >= 0x08108000 && offset <= 0x0810Bfff)
    {
        //sector_num = 14;
        sector_num = 0x12u << 3;
    }
    else if(offset >= 0x0810C000 && offset <= 0x0810Ffff)
    {
        //sector_num = 15;
        sector_num = 0x13u << 3;
    }
    else if(offset >= 0x08110000 && offset <= 0x0811Ffff)
    {
        //sector_num = 16;
        sector_num = 0x14u << 3;
    }
    else if(offset >= 0x08120000 && offset <= 0x0813Ffff)
    {
        //sector_num = 17;
        sector_num = 0x15u << 3;
    }
    else if(offset >= 0x08140000 && offset <= 0x0815Ffff)
    {
        //sector_num = 18;
        sector_num = 0x16u << 3;
    }
    else if(offset >= 0x08160000 && offset <= 0x0817Ffff)
    {
        //sector_num = 19;
        sector_num = 0x17u << 3;
    }
    else if(offset >= 0x08180000 && offset <= 0x0819Ffff)
    {
        //sector_num = 20;
        sector_num = 0x18u << 3;
    }
    else if(offset >= 0x081A0000 && offset <= 0x081BFfff)
    {
        //sector_num = 21;
        sector_num = 0x19u << 3;
    }
    else if(offset >= 0x081C0000 && offset <= 0x081DFfff)
    {
        //sector_num = 22;
        sector_num = 0x1au << 3;
    }
    else if(offset >= 0x081E0000 && offset <= 0x081FFfff)
    {
        //sector_num = 23;
        sector_num = 0x1bu << 3;
    }
    else if(offset >= 0x08200000 && offset <= 0x0823Ffff)
    {
        //sector_num = 24;
        sector_num = 0xcu << 3;
    }
    else if(offset >= 0x08240000 && offset <= 0x0827Ffff)
    {
        //sector_num = 25;
        sector_num = 0xdu << 3;
    }
    else if(offset >= 0x08280000 && offset <= 0x082BFfff)
    {
        //sector_num = 26;
        sector_num = 0xeu << 3;
    }
    else if(offset >= 0x082C0000 && offset <= 0x082FFfff)
    {
        //sector_num = 27;
        sector_num = 0xfu << 3;
    }
    else
    {
        return 1;
    }

    while (FMC_STAT & (0x01u<<16));
    FMC_CTL |= ((sector_num) | (0x01<<1));
    FMC_CTL |= (0x01u<<16);
    while (FMC_STAT & (0x01u<<16));
    FMC_CTL &= ~(0x1f<<3 | 0x01<<1);

    if(FMC_STAT & 0x1d2)
        return 1;

    return 0;
}