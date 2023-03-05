#include "gd32f4xx.h"
#include <stdio.h>
#include "littleshell.h"
#include "utilities.h"
#include "debug.h"

unsigned int dumpreg(char argc,char ** argv)
{
    unsigned int addr = 0;
    unsigned int len = 0;

    addr = myatohexuint(argv[1]);
    len = atoi(argv[2]);

    trace_word_stream(((uint32_t *)(uint32_t)(addr)),len);

    return 1;
}
LTSH_FUNCTION_EXPORT(dumpreg,"dump reg value");


unsigned int dumpaddr(char argc,char ** argv)
{
    unsigned int addr = 0;

    if(argc != 2)
        return 0;

    addr = myatohexuint(argv[1]);

    printf("0x%x -> [0x%08x] \r\n",addr,REG32(addr));

    return 1;
}
LTSH_FUNCTION_EXPORT(dumpaddr,"dump addr value");

void print_num(int count, ...)
{
    int i;
    int * args;
    args = &count +1;

    for(i = 0;i < count;i++)
    {
        printf("args:%d\r\n",*args);
        args++;
    }
}

void my_printf(char *fmt,...)
{
    va_list args;
    va_start(args,fmt);
    vprintf(fmt,args);
    va_end(args);
}

#define LOG(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define DEBUG(...) printf(__VA_ARGS__)

unsigned int args(char argc,char ** argv)
{

    print_num(5,1,2,3,4,5);
    my_printf("my printf %d\r\n",100);
    LOG("Hello! I'm %s\r\n","Qihengchuan");
    DEBUG("Hello! I'm %s\r\n","Qihengchuan");

    return 1;
}
LTSH_FUNCTION_EXPORT(args,"test args");


unsigned int section(char argc,char ** argv)
{
    #pragma section = ".bss"
    #pragma section = ".data"
    #pragma section = ".data_init"
    #pragma section = ".text"
    #pragma section = ".rodata"
    #pragma section = ".rodata.name"
    #pragma section = "FSymTab"
    #pragma section = "ASM_TEST_CODE"
    #pragma section = "CODE"

    static int i = 100;
    printf("%d\r\n",i/argc);

    /* .rodata */
    printf(".rodata start 0x%x end 0x%x size 0x%x\r\n",__section_begin(".rodata"),__section_end(".rodata"),__section_size(".rodata"));
    /* .rodata.name */
    printf(".rodata.name start 0x%x end 0x%x size 0x%x\r\n",__section_begin(".rodata.name"),__section_end(".rodata.name"),__section_size(".rodata.name"));
    /* FSymTab */
    printf("FSymTab start 0x%x end 0x%x size 0x%x\r\n",__section_begin("FSymTab"),__section_end("FSymTab"),__section_size("FSymTab"));
    /* .text */
    printf(".text start 0x%x end 0x%x size 0x%x\r\n",__section_begin(".text"),__section_end(".text"),__section_size(".text"));
    /* ASM_TEST_CODE */
    printf("ASM_TEST_CODE start 0x%x end 0x%x size 0x%x\r\n",__section_begin("ASM_TEST_CODE"),__section_end("ASM_TEST_CODE"),__section_size("ASM_TEST_CODE"));
    /* CODE */
    printf("CODE start 0x%x end 0x%x size 0x%x\r\n",__section_begin("CODE"),__section_end("CODE"),__section_size("CODE"));
    /* .bss */
    printf(".bss start 0x%x end 0x%x size 0x%x\r\n",__section_begin(".bss"),__section_end(".bss"),__section_size(".bss"));
    /* .data */
    printf(".data start 0x%x end 0x%x size 0x%x\r\n",__section_begin(".data"),__section_end(".data"),__section_size(".data"));
    /* .data_init */
    printf(".data_init start 0x%x end 0x%x size 0x%x\r\n",__section_begin(".data_init"),__section_end(".data_init"),__section_size(".data_init"));
    return 1;
}
LTSH_FUNCTION_EXPORT(section,"test args");


unsigned int status(char argc,char ** argv)
{
    uint32_t bootmode;
    bootmode = REG32(SYSCFG_BASE);

    printf("tick %d\r\n",xTaskGetTickCount());
    printf("boot mode %s\r\n",(GET_BITS(bootmode,0,2) == 0) ? "flash" :
                              ((GET_BITS(bootmode,0,2) == 1) ? "system" :
                              ((GET_BITS(bootmode,0,2) == 2) ? "sram1" :
                              ((GET_BITS(bootmode,0,2) == 3) ? "sram2" : "Unknow"))));

    return 1;
}
LTSH_FUNCTION_EXPORT(status,"show system status");

struct stu1{
uint32_t a1;
uint8_t a2[5];
uint32_t a3;
};
struct stu1 my_stu1 @ 0X20001000 = {0};

struct stu2{
uint8_t a1;
uint8_t a2[5];
uint32_t a3;
};
struct stu2 my_stu2 @ 0X20001100 = {0};


unsigned int aligen(char argc,char ** argv)
{
    uint32_t i = 0;
    uint8_t x1 = 100;

    for(i = 0;i < 1000;i++)
    {
        my_stu1.a2[0] += x1;
        if( my_stu1.a2[0] > 200)
            my_stu1.a2[0] = 0;
    }

    for(i = 0;i < 1000;i++)
    {
        my_stu2.a2[0] += x1;
        if( my_stu2.a2[0] > 200)
            my_stu2.a2[0] = 0;
    }

    return 1;
}
LTSH_FUNCTION_EXPORT(aligen,"show aligen test");


unsigned int layout(char argc,char ** argv)
{
    #pragma section = ".intvec"
    #pragma section = ".text"
    #pragma section = ".rodata"
    #pragma section = ".rodata.name"
    #pragma section = "ASM_TEST_CODE"
    #pragma section = "FSymTab"

    /* .intvec */
    printf("[.intvec]\t [0x%x~0x%x:0x%x]\r\n",__section_begin(".intvec"),__section_end(".intvec"),__section_size(".intvec"));
    /* .text */
    printf("[.text]\t\t [0x%x~0x%x:0x%x]\r\n",__section_begin(".text"),__section_end(".text"),__section_size(".text"));
    /* .rodata */
    printf("[.rodata]\t [0x%x~0x%x:0x%x]\r\n",__section_begin(".rodata"),__section_end(".rodata"),__section_size(".rodata"));
    /* .rodata.name */
    printf("[.rodata.name]\t [0x%x~0x%x:0x%x]\r\n",__section_begin(".rodata.name"),__section_end(".rodata.name"),__section_size(".rodata.name"));
    /* ASM_TEST_CODE */
    printf("[ASM_TEST_CODE]\t [0x%x~0x%x:0x%x]\r\n",__section_begin("ASM_TEST_CODE"),__section_end("ASM_TEST_CODE"),__section_size("ASM_TEST_CODE"));
    /* FSymTab */
    printf("[FSymTab]\t [0x%x~0x%x:0x%x]\r\n",__section_begin("FSymTab"),__section_end("FSymTab"),__section_size("FSymTab"));
    return 1;
}
LTSH_FUNCTION_EXPORT(layout,"layout section infomation");