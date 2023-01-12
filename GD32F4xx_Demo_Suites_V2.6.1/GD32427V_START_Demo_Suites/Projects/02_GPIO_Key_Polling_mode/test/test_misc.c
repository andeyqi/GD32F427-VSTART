#include "gd32f4xx.h"
#include <stdio.h>
#include "littleshell.h"
#include "utilities.h"

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

    /* BSS */
    printf("BSS start 0x%x end 0x%x size 0x%x\r\n",__section_begin(".bss"),__section_end(".bss"),__section_size(".bss"));
    /* DATA */
    printf("DATA start 0x%x end 0x%x size 0x%x\r\n",__section_begin(".data"),__section_end(".data"),__section_size(".data"));

    return 1;
}
LTSH_FUNCTION_EXPORT(section,"test args");