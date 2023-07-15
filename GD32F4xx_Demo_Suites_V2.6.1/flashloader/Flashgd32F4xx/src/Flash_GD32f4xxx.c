#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int mini_printf(const char *fmt, ...);

#define USE_ARGC_ARGV 1
#define DEBUG_MESS 1

#define REG32(addr)           (*(volatile uint32_t *)(uint32_t)(addr))
#define BIT(x)                       ((uint32_t)((uint32_t)0x01U<<(x)))
#define BITS(start, end)             ((0xFFFFFFFFUL << (start)) & (0xFFFFFFFFUL >> (31U - (uint32_t)(end))))
#define GET_BITS(regval, start, end) (((regval) & BITS((start),(end))) >> (start))

#define FMC_BASE              ((uint32_t)0x40020000U + 0x00003C00U)
#define RCU_BASE              ((uint32_t)0x40020000U + 0x00003800U)
#define GPIO_BASE             ((uint32_t)0x40020000U + 0x00000000U)
#define USART0                ((uint32_t)0x40000000U + 0x00004400U+0x0000CC00U)

#define FMC                        FMC_BASE
#define RCU                        RCU_BASE

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


/* registers definitions */
#define RCU_CTL                         REG32(RCU + 0x00U)        /*!< control register */
#define RCU_PLL                         REG32(RCU + 0x04U)        /*!< PLL register */
#define RCU_CFG0                        REG32(RCU + 0x08U)        /*!< clock configuration register 0 */
#define RCU_INT                         REG32(RCU + 0x0CU)        /*!< clock interrupt register */
#define RCU_AHB1RST                     REG32(RCU + 0x10U)        /*!< AHB1 reset register */
#define RCU_AHB2RST                     REG32(RCU + 0x14U)        /*!< AHB2 reset register */
#define RCU_AHB3RST                     REG32(RCU + 0x18U)        /*!< AHB3 reset register */
#define RCU_APB1RST                     REG32(RCU + 0x20U)        /*!< APB1 reset register */
#define RCU_APB2RST                     REG32(RCU + 0x24U)        /*!< APB2 reset register */
#define RCU_AHB1EN                      REG32(RCU + 0x30U)        /*!< AHB1 enable register */
#define RCU_AHB2EN                      REG32(RCU + 0x34U)        /*!< AHB2 enable register */
#define RCU_AHB3EN                      REG32(RCU + 0x38U)        /*!< AHB3 enable register */
#define RCU_APB1EN                      REG32(RCU + 0x40U)        /*!< APB1 enable register */
#define RCU_APB2EN                      REG32(RCU + 0x44U)        /*!< APB2 enable register */
#define RCU_AHB1SPEN                    REG32(RCU + 0x50U)        /*!< AHB1 sleep mode enable register */
#define RCU_AHB2SPEN                    REG32(RCU + 0x54U)        /*!< AHB2 sleep mode enable register */
#define RCU_AHB3SPEN                    REG32(RCU + 0x58U)        /*!< AHB3 sleep mode enable register */
#define RCU_APB1SPEN                    REG32(RCU + 0x60U)        /*!< APB1 sleep mode enable register */
#define RCU_APB2SPEN                    REG32(RCU + 0x64U)        /*!< APB2 sleep mode enable register */
#define RCU_BDCTL                       REG32(RCU + 0x70U)        /*!< backup domain control register */
#define RCU_RSTSCK                      REG32(RCU + 0x74U)        /*!< reset source / clock register */
#define RCU_PLLSSCTL                    REG32(RCU + 0x80U)        /*!< PLL clock spread spectrum control register */
#define RCU_PLLI2S                      REG32(RCU + 0x84U)        /*!< PLLI2S register */
#define RCU_PLLSAI                      REG32(RCU + 0x88U)        /*!< PLLSAI register */
#define RCU_CFG1                        REG32(RCU + 0x8CU)        /*!< clock configuration register 1 */
#define RCU_ADDCTL                      REG32(RCU + 0xC0U)        /*!< Additional clock control register */
#define RCU_ADDINT                      REG32(RCU + 0xCCU)        /*!< Additional clock interrupt register */
#define RCU_ADDAPB1RST                  REG32(RCU + 0xE0U)        /*!< APB1 additional reset register */
#define RCU_ADDAPB1EN                   REG32(RCU + 0xE4U)        /*!< APB1 additional enable register */
#define RCU_ADDAPB1SPEN                 REG32(RCU + 0xE8U)        /*!< APB1 additional sleep mode enable register */
#define RCU_VKEY                        REG32(RCU + 0x100U)       /*!< voltage key register */
#define RCU_DSV                         REG32(RCU + 0x134U)       /*!< deep-sleep mode voltage register */


/* GPIOx(x=A,B,C,D,E,F,G,H,I) definitions */
#define GPIOA                      (GPIO_BASE + 0x00000000U)
#define GPIOB                      (GPIO_BASE + 0x00000400U)
#define GPIOC                      (GPIO_BASE + 0x00000800U)
#define GPIOD                      (GPIO_BASE + 0x00000C00U)
#define GPIOE                      (GPIO_BASE + 0x00001000U)
#define GPIOF                      (GPIO_BASE + 0x00001400U)
#define GPIOG                      (GPIO_BASE + 0x00001800U)
#define GPIOH                      (GPIO_BASE + 0x00001C00U)
#define GPIOI                      (GPIO_BASE + 0x00002000U)

/* registers definitions */
#define GPIO_CTL(gpiox)            REG32((gpiox) + 0x00U)    /*!< GPIO port control register */
#define GPIO_OMODE(gpiox)          REG32((gpiox) + 0x04U)    /*!< GPIO port output mode register */
#define GPIO_OSPD(gpiox)           REG32((gpiox) + 0x08U)    /*!< GPIO port output speed register */
#define GPIO_PUD(gpiox)            REG32((gpiox) + 0x0CU)    /*!< GPIO port pull-up/pull-down register */
#define GPIO_ISTAT(gpiox)          REG32((gpiox) + 0x10U)    /*!< GPIO port input status register */
#define GPIO_OCTL(gpiox)           REG32((gpiox) + 0x14U)    /*!< GPIO port output control register */
#define GPIO_BOP(gpiox)            REG32((gpiox) + 0x18U)    /*!< GPIO port bit operate register */
#define GPIO_LOCK(gpiox)           REG32((gpiox) + 0x1CU)    /*!< GPIO port configuration lock register */
#define GPIO_AFSEL0(gpiox)         REG32((gpiox) + 0x20U)    /*!< GPIO alternate function selected register 0 */
#define GPIO_AFSEL1(gpiox)         REG32((gpiox) + 0x24U)    /*!< GPIO alternate function selected register 1 */
#define GPIO_BC(gpiox)             REG32((gpiox) + 0x28U)    /*!< GPIO bit clear register */
#define GPIO_TG(gpiox)             REG32((gpiox) + 0x2CU)    /*!< GPIO port bit toggle register */


/* registers definitions */
#define USART_STAT0(usartx)           REG32((usartx) + 0x00U)        /*!< USART status register 0 */
#define USART_DATA(usartx)            REG32((usartx) + 0x04U)        /*!< USART data register */
#define USART_BAUD(usartx)            REG32((usartx) + 0x08U)        /*!< USART baud rate register */
#define USART_CTL0(usartx)            REG32((usartx) + 0x0CU)        /*!< USART control register 0 */
#define USART_CTL1(usartx)            REG32((usartx) + 0x10U)        /*!< USART control register 1 */
#define USART_CTL2(usartx)            REG32((usartx) + 0x14U)        /*!< USART control register 2 */
#define USART_GP(usartx)              REG32((usartx) + 0x18U)        /*!< USART guard time and prescaler register */
#define USART_CTL3(usartx)            REG32((usartx) + 0x80U)        /*!< USART control register 3 */
#define USART_RT(usartx)              REG32((usartx) + 0x84U)        /*!< USART receiver timeout register */
#define USART_STAT1(usartx)           REG32((usartx) + 0x88U)        /*!< USART status register 1 */
#define USART_CHC(usartx)             REG32((usartx) + 0xC0U)        /*!< USART coherence control register */

static __no_init uint32_t parallelism;
static __no_init uint8_t blankcheck;
typedef enum
{
    CK_SYS      = 0,                                                        /*!< system clock */
    CK_AHB,                                                                 /*!< AHB clock */
    CK_APB1,                                                                /*!< APB1 clock */
    CK_APB2,                                                                /*!< APB2 clock */
}rcu_clock_freq_enum;

#define SEL_IRC16M      0x00U
#define SEL_HXTAL       0x01U
#define SEL_PLLP        0x02U

/* RCU_PLL */
#define RCU_PLL_PLLPSC                  BITS(0,5)                 /*!< The PLL VCO source clock prescaler */
#define RCU_PLL_PLLN                    BITS(6,14)                /*!< The PLL VCO clock multi factor */
#define RCU_PLL_PLLP                    BITS(16,17)               /*!< The PLLP output frequency division factor from PLL VCO clock */
#define RCU_PLL_PLLSEL                  BIT(22)                   /*!< PLL Clock Source Selection */
#define RCU_PLL_PLLQ                    BITS(24,27)               /*!< The PLL Q output frequency division factor from PLL VCO clock */

#define RCU_PLLSRC_HXTAL                RCU_PLL_PLLSEL
#define IRC16M_VALUE  ((uint32_t)16000000)
#define HXTAL_VALUE   ((uint32_t)25000000)

#define USART_CTL0_OVSMOD             BIT(15)
#define USART_BAUD_FRADIV             BITS(0,3)    /*!< fraction part of baud-rate divider */
#define USART_BAUD_INTDIV             BITS(4,15)   /*!< integer part of baud-rate divider */

#ifdef DEBUG_MESS
void send_char(char c)
{
  USART_DATA(USART0) = c;
  while(!(USART_STAT0(USART0) & (0X01 << 6)));
}

void uartsendstring(char * s)
{
  uint32_t len = strlen(s);
  uint32_t i = 0;
  for(i = 0; i < len;i++)
  {
    USART_DATA(USART0) = s[i];
    while(!(USART_STAT0(USART0) & (0X01 << 6)));
  }

}
#endif // DEBUG_MESS


uint32_t rcu_clock_freq_get(rcu_clock_freq_enum clock)
{
    uint32_t sws, ck_freq = 0U;
    uint32_t cksys_freq, ahb_freq, apb1_freq, apb2_freq;
    uint32_t pllpsc, plln, pllsel, pllp, ck_src, idx, clk_exp;

    /* exponent of AHB, APB1 and APB2 clock divider */
    uint8_t ahb_exp[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
    uint8_t apb1_exp[8] = {0, 0, 0, 0, 1, 2, 3, 4};
    uint8_t apb2_exp[8] = {0, 0, 0, 0, 1, 2, 3, 4};

    sws = GET_BITS(RCU_CFG0, 2, 3);
    switch(sws) {
    /* IRC16M is selected as CK_SYS */
    case SEL_IRC16M:
        cksys_freq = IRC16M_VALUE;
        break;
    /* HXTAL is selected as CK_SYS */
    case SEL_HXTAL:
        cksys_freq = HXTAL_VALUE;
        break;
    /* PLLP is selected as CK_SYS */
    case SEL_PLLP:
        /* get the value of PLLPSC[5:0] */
        pllpsc = GET_BITS(RCU_PLL, 0U, 5U);
        plln = GET_BITS(RCU_PLL, 6U, 14U);
        pllp = (GET_BITS(RCU_PLL, 16U, 17U) + 1U) * 2U;
        /* PLL clock source selection, HXTAL or IRC16M/2 */
        pllsel = (RCU_PLL & RCU_PLL_PLLSEL);
        if(RCU_PLLSRC_HXTAL == pllsel) {
            ck_src = HXTAL_VALUE;
        } else {
            ck_src = IRC16M_VALUE;
        }
        cksys_freq = ((ck_src / pllpsc) * plln) / pllp;
        break;
    /* IRC16M is selected as CK_SYS */
    default:
        cksys_freq = IRC16M_VALUE;
        break;
    }
    /* calculate AHB clock frequency */
    idx = GET_BITS(RCU_CFG0, 4, 7);
    clk_exp = ahb_exp[idx];
    ahb_freq = cksys_freq >> clk_exp;

    /* calculate APB1 clock frequency */
    idx = GET_BITS(RCU_CFG0, 10, 12);
    clk_exp = apb1_exp[idx];
    apb1_freq = ahb_freq >> clk_exp;

    /* calculate APB2 clock frequency */
    idx = GET_BITS(RCU_CFG0, 13, 15);
    clk_exp = apb2_exp[idx];
    apb2_freq = ahb_freq >> clk_exp;

    /* return the clocks frequency */
    switch(clock) {
    case CK_SYS:
        ck_freq = cksys_freq;
        break;
    case CK_AHB:
        ck_freq = ahb_freq;
        break;
    case CK_APB1:
        ck_freq = apb1_freq;
        break;
    case CK_APB2:
        ck_freq = apb2_freq;
        break;
    default:
        break;
    }
    return ck_freq;
}

void execUserFlashInit(void)
{
    /* unlock flash */
    FMC_KEY = 0x45670123;
    FMC_KEY = 0xCDEF89AB;
    FMC_WS = 0x00000000;
    FMC_CTL = 0x00000000;
    FMC_STAT = 0x000000F0;
}


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

#ifdef DEBUG_MESS
    /* enable GPIO clock */
    RCU_AHB1EN |= 0x01u << 1;
    /* enable USART clock */
    RCU_APB2EN |= 0x01u <<4;

    GPIO_CTL(GPIOB) &= ~(0x03u << (2*6));
    GPIO_CTL(GPIOB) |= (2 << (2*6));

    GPIO_PUD(GPIOB) &= ~(0x03u << (2*6));
    GPIO_PUD(GPIOB) |= (1u << (2*6));
    GPIO_OMODE(GPIOB) &= ~(0x01u << 6);

    GPIO_OSPD(GPIOB) &= ~(0x03u << (2*6));
    GPIO_OSPD(GPIOB) |= (0x01u << (2*6));

    GPIO_AFSEL0(GPIOB) &= ~(0xf << (4*6));
    GPIO_AFSEL0(GPIOB) |= (0x7 << (4*6));

    /* config uart0 */
    uint32_t uclk = 0U, intdiv = 0U, fradiv = 0U, udiv = 0U;
    uclk = rcu_clock_freq_get(CK_APB2);

    if(USART_CTL0(USART0) & USART_CTL0_OVSMOD) {
        /* when oversampling by 8, configure the value of USART_BAUD */
        udiv = ((2U * uclk) + 115200 / 2U) / 115200;
        intdiv = udiv & 0xfff0U;
        fradiv = (udiv >> 1U) & 0x7U;
        USART_BAUD(USART0) = ((USART_BAUD_FRADIV | USART_BAUD_INTDIV) & (intdiv | fradiv));
    } else {
        /* when oversampling by 16, configure the value of USART_BAUD */
        udiv = (uclk + 115200 / 2U) / 115200;
        intdiv = udiv & 0xfff0U;
        fradiv = udiv & 0xfU;
        USART_BAUD(USART0) = ((USART_BAUD_FRADIV | USART_BAUD_INTDIV) & (intdiv | fradiv));
    }

    USART_CTL0(USART0) &= ~(0x01u << 10);
    USART_CTL0(USART0) &= ~(0x01u << 12);

    USART_CTL1(USART0) &= ~(0x03u << 12);

    USART_CTL0(USART0) |= (0x01u << 3);
    USART_CTL0(USART0) |= (0x01u << 13);


    mini_printf("\r\nuart init ok.\r\n");
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

    mini_printf("flash write dst %p %d %d.\r\n",dst,offset_into_block,count);

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

    mini_printf("flash erase %x.\r\n",offset);

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