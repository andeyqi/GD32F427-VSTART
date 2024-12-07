#ifndef HF_PRINTLN

#include <stdio.h>
#include <stdlib.h>

#define HF_PRINTLN(format, ...) do { \
    printf(format, ##__VA_ARGS__); \
    printf("\r\n"); \
} while (0)

#endif

/********************************************************************************************************
 *                                  Private Function Declarations                                       *
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
  stacked_r0 = ((unsigned long) hardfault_args[0]);
  stacked_r1 = ((unsigned long) hardfault_args[1]);
  stacked_r2 = ((unsigned long) hardfault_args[2]);
  stacked_r3 = ((unsigned long) hardfault_args[3]);
  stacked_r12 = ((unsigned long) hardfault_args[4]);
  stacked_lr = ((unsigned long) hardfault_args[5]);
  stacked_pc = ((unsigned long) hardfault_args[6]);
  stacked_psr = ((unsigned long) hardfault_args[7]);
  HF_PRINTLN ("[Hard fault handler - all numbers in hex]");
  HF_PRINTLN ("R0 = %08x", stacked_r0);
  HF_PRINTLN ("R1 = %08x", stacked_r1);
  HF_PRINTLN ("R2 = %08x", stacked_r2);
  HF_PRINTLN ("R3 = %08x", stacked_r3);
  HF_PRINTLN ("R12 = %08x", stacked_r12);
  HF_PRINTLN ("LR [R14] = %08x subroutine call return address", stacked_lr);
  HF_PRINTLN ("PC [R15] = %08x program counter", stacked_pc);
  HF_PRINTLN ("PSR = %08x", stacked_psr);
  HF_PRINTLN ("BFAR = %08x", (*((volatile unsigned long *)(0xE000ED38))));
  HF_PRINTLN ("CFSR = %08x", (*((volatile unsigned long *)(0xE000ED28))));
  HF_PRINTLN ("HFSR = %08x", (*((volatile unsigned long *)(0xE000ED2C))));
  HF_PRINTLN ("DFSR = %08x", (*((volatile unsigned long *)(0xE000ED30))));
  HF_PRINTLN ("AFSR = %08x", (*((volatile unsigned long *)(0xE000ED3C))));
  HF_PRINTLN ("SCB_SHCSR = %08x", (*((volatile unsigned long *)(0xE000ED24))));
  while (1);
}

/********************************************************************************************************
 *                                  Global Function Declarations                                        *
 *******************************************************************************************************/
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
