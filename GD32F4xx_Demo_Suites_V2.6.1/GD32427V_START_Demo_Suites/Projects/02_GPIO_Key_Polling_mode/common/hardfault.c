#ifndef HF_PRINTLN

#include <stdio.h>
#include <stdlib.h>

#define HF_PRINTLN(format, ...) do { \
    printf(format, ##__VA_ARGS__); \
    printf("\r\n"); \
} while (0)

#define HF_PRINT(format, ...)     printf(format, ##__VA_ARGS__)

#endif

/********************************************************************************************************
 *                                    Private Type Declarations                                         *
 *******************************************************************************************************/
typedef struct _hard_fault_status_regs{
    unsigned int cfsr;
    unsigned int hfsr;
    unsigned int mmar;
    unsigned int bfar;
    unsigned int cfsr_mfsr;
    unsigned int cfsr_bfsr;
    unsigned int cfsr_ufsr;
}hard_fault_status_regs_t;

/********************************************************************************************************
 *                                  Private Function Declarations                                       *
 *******************************************************************************************************/
static void usage_fault_track(hard_fault_status_regs_t * regs)
{
    HF_PRINTLN("usage fault:");
    HF_PRINT("SCB_CFSR_UFSR:0x%02X ", regs->cfsr_ufsr);

    if(regs->cfsr_ufsr & (1<<0u))
    {
        /* [0]:UNDEFINSTR */
        HF_PRINT("UNDEFINSTR ");
    }

    if(regs->cfsr_ufsr & (1<<1u))
    {
        /* [1]:INVSTATE */
        HF_PRINT("INVSTATE ");
    }

    if(regs->cfsr_ufsr & (1<<2u))
    {
        /* [2]:INVPC */
        HF_PRINT("INVPC ");
    }

    if(regs->cfsr_ufsr & (1<<3u))
    {
        /* [3]:NOCP */
        HF_PRINT("NOCP ");
    }

    if(regs->cfsr_ufsr & (1<<8u))
    {
        /* [8]:UNALIGNED */
        HF_PRINT("UNALIGNED ");
    }

    if(regs->cfsr_ufsr & (1<<9u))
    {
        /* [9]:DIVBYZERO */
        HF_PRINT("DIVBYZERO ");
    }

    HF_PRINTLN("");
}

static void bus_fault_track(hard_fault_status_regs_t * regs)
{
    HF_PRINTLN("bus fault:");
    HF_PRINT("SCB_CFSR_BFSR:0x%02X ", regs->cfsr_bfsr);

    if(regs->cfsr_bfsr & (1<<0u))
    {
        /* [0]:IBUSERR */
        HF_PRINT("IBUSERR ");
    }

    if(regs->cfsr_bfsr & (1<<1u))
    {
        /* [1]:PRECISERR */
        HF_PRINT("PRECISERR ");
    }

    if(regs->cfsr_bfsr & (1<<2u))
    {
        /* [2]:IMPRECISERR */
        HF_PRINT("IMPRECISERR ");
    }

    if(regs->cfsr_bfsr & (1<<3u))
    {
        /* [3]:UNSTKERR */
        HF_PRINT("UNSTKERR ");
    }

    if(regs->cfsr_bfsr & (1<<4u))
    {
        /* [4]:STKERR */
        HF_PRINT("STKERR ");
    }

    if(regs->cfsr_bfsr & (1<<7u))
    {
        HF_PRINTLN("SCB->BFAR:%08X", regs->cfsr_bfsr);
    }
    else
    {
        HF_PRINTLN("");
    }
}

static void mem_manage_fault_track(hard_fault_status_regs_t * regs)
{
    HF_PRINTLN("mem manage fault:");
    HF_PRINT("SCB_CFSR_MFSR:0x%02X ", regs->cfsr_mfsr);

    if(regs->cfsr_mfsr & (1<<0u))
    {
        /* [0]:IACCVIOL */
        HF_PRINT("IACCVIOL ");
    }

    if(regs->cfsr_mfsr & (1<<1u))
    {
        /* [1]:DACCVIOL */
        HF_PRINT("DACCVIOL ");
    }

    if(regs->cfsr_mfsr & (1<<3u))
    {
        /* [3]:MUNSTKERR */
        HF_PRINT("MUNSTKERR ");
    }

    if(regs->cfsr_mfsr & (1<<4u))
    {
        /* [4]:MSTKERR */
        HF_PRINT("MSTKERR ");
    }

    if(regs->cfsr_mfsr & (1<<7u))
    {
        /* [7]:MMARVALID */
        HF_PRINTLN("SCB->MMAR:%08X", regs->mmar);
    }
    else
    {
        HF_PRINTLN("");
    }
}

static void hard_fault_track(hard_fault_status_regs_t * regs)
{
    if(regs->hfsr & (1UL<<1U))
    {
        /* [1]:VECTBL, Indicates hard fault is caused by failed vector fetch. */
        HF_PRINTLN("failed vector fetch");
    }

    if(regs->hfsr & (1UL<<30U))
    {
        /* [30]:FORCED, Indicates hard fault is taken because of bus fault,
                        memory management fault, or usage fault. */
        if(regs->cfsr_bfsr)
        {
            bus_fault_track(regs);
        }
        if(regs->cfsr_mfsr)
        {
            mem_manage_fault_track(regs);
        }
        if(regs->cfsr_ufsr)
        {
            usage_fault_track(regs);
        }
    }

    if(regs->hfsr & (1UL<<31U))
    {
        /* [31]:DEBUGEVT, Indicates hard fault is triggered by debug event. */
        HF_PRINTLN("debug event");
    }
}
/********************************************************************************************************
 *                                  Global Function Declarations                                        *
 *******************************************************************************************************/
void hard_fault_handler_c (unsigned int * hardfault_args)
{
  unsigned int stacked_r0;
  unsigned int stacked_r1;
  unsigned int stacked_r2;
  unsigned int stacked_r3;
  unsigned int stacked_r12;
  unsigned int stacked_lr;
  unsigned int stacked_pc;
  unsigned int stacked_psr;
  hard_fault_status_regs_t regs;
  stacked_r0 = ((unsigned long) hardfault_args[0]);
  stacked_r1 = ((unsigned long) hardfault_args[1]);
  stacked_r2 = ((unsigned long) hardfault_args[2]);
  stacked_r3 = ((unsigned long) hardfault_args[3]);
  stacked_r12 = ((unsigned long) hardfault_args[4]);
  stacked_lr = ((unsigned long) hardfault_args[5]);
  stacked_pc = ((unsigned long) hardfault_args[6]);
  stacked_psr = ((unsigned long) hardfault_args[7]);

  regs.hfsr = (*(volatile const unsigned *)0xE000ED2C); /* HardFault Status Register */
  regs.cfsr = (*(volatile const unsigned *)0xE000ED28); /* Configurable Fault Status Register */
  regs.mmar = (*(volatile const unsigned *)0xE000ED34); /* MemManage Fault Address register */
  regs.bfar = (*(volatile const unsigned *)0xE000ED38); /* Bus Fault Address Register */
  regs.cfsr_ufsr = (*(volatile const unsigned short*)0xE000ED2A); /* Usage Fault Status Register */
  regs.cfsr_mfsr = (*(volatile const unsigned char*)0xE000ED28);  /* Memory-management Fault Status Register */
  regs.cfsr_bfsr = (*(volatile const unsigned char*)0xE000ED29);  /* Bus Fault Status Register */
  HF_PRINTLN ("[Hard fault handler - all numbers in hex]");
  HF_PRINTLN ("R0 = %08x", stacked_r0);
  HF_PRINTLN ("R1 = %08x", stacked_r1);
  HF_PRINTLN ("R2 = %08x", stacked_r2);
  HF_PRINTLN ("R3 = %08x", stacked_r3);
  HF_PRINTLN ("R12 = %08x", stacked_r12);
  HF_PRINTLN ("LR [R14] = %08x subroutine call return address", stacked_lr);
  HF_PRINTLN ("PC [R15] = %08x program counter", stacked_pc);
  HF_PRINTLN ("PSR = %08x", stacked_psr);
  HF_PRINTLN ("BFAR = %08x", regs.bfar);
  HF_PRINTLN ("CFSR = %08x", regs.cfsr);
  HF_PRINTLN ("HFSR = %08x", regs.hfsr);
  HF_PRINTLN ("DFSR = %08x", (*((volatile unsigned long *)(0xE000ED30))));
  HF_PRINTLN ("AFSR = %08x", (*((volatile unsigned long *)(0xE000ED3C))));
  HF_PRINTLN ("SCB_SHCSR = %08x", (*((volatile unsigned long *)(0xE000ED24))));

  hard_fault_track(&regs);
  while (1);
}

/**
  * @brief This function handles HardFault.
  */
__attribute__((naked))
void HardFault_Handler(void)
{
  __asm volatile(
      "tst lr, #4 \n"
      "ite eq \n"
      "mrseq r0, msp \n"
      "mrsne r0, psp \n"
      "b hard_fault_handler_c \n");
}
