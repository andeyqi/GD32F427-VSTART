#include "asm_test.h"
#include "utilities.h"
#include "littleshell.h"
#include <stdint.h>
#include <stdio.h>

unsigned int asmtest(char argc,char ** argv)
{
    int cmd  = 0;

    if(argc == 1 )
        return 0;
    
    cmd = myatoi(argv[1]);
    switch(cmd)
    {
    case 0:
        asm_test_bfi();
        break;
    case 1:
        asm_test_ror();
        break;
    case 2:
        asm_test_mov();
        break;
    case 3:
        asm_test_mvn();
        break;
    case 4:
        asm_test_asr();
        break;
    case 5:
        asm_test_add();
        break;
    default:
        break;
    }
    
    return 1;
}
LTSH_FUNCTION_EXPORT(asmtest,"dump addr value");

typedef struct
{

    uint8_t byte0 : 8; 
    uint8_t byte1 : 8; 
    uint8_t byte2 : 8; 
    uint8_t byte3_bit0_3: 4;
    uint8_t byte3_bit4 : 1;
    uint8_t byte3_bit5_7 : 3;

}__attribute__((packed)) bit_test;


unsigned int asmctest(char argc,char ** argv)
{
    int cmd  = 0;
    int32_t asr = 0xaabb;
    uint32_t uasr = 0x55aa;
    bit_test bit_domain;
    unsigned long long adc;

    if(argc == 1 )
        return 0;
    
    cmd = myatoi(argv[1]);
    switch(cmd)
    {
    case 0:
        bit_domain.byte3_bit0_3 = myatoi(argv[2]);
        printf("asm test c bfi %x \r\n",bit_domain.byte3_bit0_3);
        break; 
    case 1:
        printf("%x %x %x\r\n",asr/2,asr>>4,uasr>>5);
        break;
    case 2:
        adc = myatol(argv[3]) + myatol(argv[2]);
        printf("%llx \r\n",adc);
    default:
        break;
    }
    
    return 1;
}
LTSH_FUNCTION_EXPORT(asmctest,"test c sam");