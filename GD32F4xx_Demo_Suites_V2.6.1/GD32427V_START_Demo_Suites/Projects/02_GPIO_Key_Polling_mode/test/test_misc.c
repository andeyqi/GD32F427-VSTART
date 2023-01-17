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