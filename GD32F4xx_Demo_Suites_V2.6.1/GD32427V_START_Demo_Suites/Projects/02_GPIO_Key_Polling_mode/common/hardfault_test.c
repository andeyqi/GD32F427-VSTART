
/* 
 * fault_test.c 
 * 
 *  Created on: 2016/12/25 
 *      Author: Armink 
 */  
  
#include <stdio.h>  
#include "littleshell.h"  
  
void fault_test_by_unalign(void) {  
    volatile int * SCB_CCR = (volatile int *) 0xE000ED14; // SCB->CCR  
    volatile int * p;  
    volatile int value;  
  
    *SCB_CCR |= (1 << 3); /* bit3: UNALIGN_TRP. */  
  
    p = (int *) 0x00;  
    value = *p;  
    printf("addr:0x%02X value:0x%08X\r\n", (int) p, value);  
  
    p = (int *) 0x04;  
    value = *p;  
    printf("addr:0x%02X value:0x%08X\r\n", (int) p, value);  
  
    p = (int *) 0x03;  
    value = *p;  
    printf("addr:0x%02X value:0x%08X\r\n", (int) p, value);  
}  
  
int hard1(char argc,char ** argv)  
{  
  fault_test_by_unalign();  
  return 0;  
}  
  
LTSH_FUNCTION_EXPORT(hard1,"test hard fault1")  
  
  
void fault_test_by_div0(void) {  
    volatile int * SCB_CCR = (volatile int *) 0xE000ED14; // SCB->CCR  
    int x, y, z;  
  
    *SCB_CCR |= (1 << 4); /* bit4: DIV_0_TRP. */  
  
    x = 10;  
    y = 0;  
    z = x / y;  
    printf("z:%d\n", z);  
}  
  
  
int hard2(char argc,char ** argv)  
{  
  fault_test_by_div0();  
  return 0;  
}  
  
LTSH_FUNCTION_EXPORT(hard2,"test hard fault2")
