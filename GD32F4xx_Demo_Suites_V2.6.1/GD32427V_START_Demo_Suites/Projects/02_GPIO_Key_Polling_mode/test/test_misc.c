#include "gd32f4xx.h"
#include <stdio.h>
#include "littleshell.h"
#include "utilities.h"
#include "debug.h"
#include "FreeRTOS.h"
#include "task.h"

unsigned int dumpreg(char argc,char ** argv)
{
    unsigned int addr = 0;
    unsigned int len = 0;

    addr = myatohexuint(argv[1]);
    len = atoi(argv[2]);

    trace_word_stream(((uint32_t *)(uint32_t)(addr)),len,0x00);

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
    #pragma section = "CODE"
    #pragma section = ".iar.init_table"
    #pragma section = ".data_init"

    /* .intvec */
    printf("[.intvec]\t\t [0x%x~0x%x:0x%x]\r\n",__section_begin(".intvec"),__section_end(".intvec"),__section_size(".intvec"));
    /* .text */
    printf("[.text]\t\t\t [0x%x~0x%x:0x%x]\r\n",__section_begin(".text"),__section_end(".text"),__section_size(".text"));
    /* .rodata */
    printf("[.rodata]\t\t [0x%x~0x%x:0x%x]\r\n",__section_begin(".rodata"),__section_end(".rodata"),__section_size(".rodata"));
    /* .rodata.name */
    printf("[.rodata.name]\t\t [0x%x~0x%x:0x%x]\r\n",__section_begin(".rodata.name"),__section_end(".rodata.name"),__section_size(".rodata.name"));
    /* ASM_TEST_CODE */
    printf("[ASM_TEST_CODE]\t\t [0x%x~0x%x:0x%x]\r\n",__section_begin("ASM_TEST_CODE"),__section_end("ASM_TEST_CODE"),__section_size("ASM_TEST_CODE"));
    /* FSymTab */
    printf("[FSymTab]\t\t [0x%x~0x%x:0x%x]\r\n",__section_begin("FSymTab"),__section_end("FSymTab"),__section_size("FSymTab"));
    /* CODE */
    printf("[CODE]\t\t\t [0x%x~0x%x:0x%x]\r\n",__section_begin("CODE"),__section_end("CODE"),__section_size("CODE"));
    /* .iar.init_table */
    printf("[.iar.init_table]\t [0x%x~0x%x:0x%x]\r\n",__section_begin(".iar.init_table"),__section_end(".iar.init_table"),__section_size(".iar.init_table"));
    /* .data_init */
    printf("[.data_init]\t\t [0x%x~0x%x:0x%x]\r\n",__section_begin(".data_init"),__section_end(".data_init"),__section_size(".data_init"));
    return 1;
}
LTSH_FUNCTION_EXPORT(layout,"layout section infomation");


extern int mini_printf(const char *fmt, ...);

unsigned int test_mprintf(char argc,char ** argv)
{
    mini_printf("mini printf\n");
    mini_printf("%d %x %08d\n",100,0x100,10);
    mini_printf("%f %x %08d\n",3.14f,0x100,10);

    return 1;
}
LTSH_FUNCTION_EXPORT(test_mprintf,"test mini printf");

unsigned int mul(char argc,char ** argv)
{
    int a,b;

    a = atoi(argv[1]);
    b = atoi(argv[2]);

    printf("%s * %s = %d \r\n",argv[1],argv[2],a*b);

    return 1;
}
LTSH_FUNCTION_EXPORT(mul,"mul a& b");

unsigned int mydiv(char argc,char ** argv)
{
    float a,b;

    a = (float)atoi(argv[1]);
    b = (float)atoi(argv[2]);

    printf("%s / %s = %f \r\n",argv[1],argv[2],(double)(a/b));

    return 1;
}
LTSH_FUNCTION_EXPORT(mydiv,"mul a& b");

unsigned int add(char argc,char ** argv)
{
    int a,b;

    a = atoi(argv[1]);
    b = atoi(argv[2]);

    printf("%s + %s = %d \r\n",argv[1],argv[2],a+b);

    return 1;
}
LTSH_FUNCTION_EXPORT(add,"mul a& b");

unsigned int sub(char argc,char ** argv)
{
    int a,b;

    a = atoi(argv[1]);
    b = atoi(argv[2]);

    printf("%s - %s = %d \r\n",argv[1],argv[2],a-b);

    return 1;
}
LTSH_FUNCTION_EXPORT(sub,"mul a& b");


unsigned int iccarm(char argc,char ** argv)
{
    printf("__ARM_ARCH = %d\r\n",__ARM_ARCH);
    printf("__ARM_ARCH_PROFILE = %c\r\n",__ARM_ARCH_PROFILE);
    printf("__ARM_FP = %d\r\n",__ARM_FP);
    printf("__ARM_ACLE = %d\r\n",__ARM_ACLE );
    printf("__ARM_ARCH_ISA_THUMB = %d\r\n",__ARM_ARCH_ISA_THUMB);
#if defined (__ARM_FEATURE_UNALIGNED)
    printf("__ARM_FEATURE_UNALIGNED is surport\r\n");
#else
    printf("__ARM_FEATURE_UNALIGNED is not surport\r\n");
#endif

#if defined (__ICCARM__)
    printf("__VER__ == %d.\r\n",__VER__);
#endif
    return 0;
}
LTSH_FUNCTION_EXPORT(iccarm,"iar c complier");


#include "byte_queue.h"

unsigned int byte_queue_test(char argc,char ** argv)
{
    uint8_t cmd = atoi(argv[1]);
    static byte_queue_t * queue;
    static uint8_t buff[20];

    switch(cmd)
    {
    case 0:
        queue = __new_class(byte_queue,buff,20);
        break;
    case 1:
        BYTE_QUEUE.Depose(queue);
        break;
    }
    return 0;
}
LTSH_FUNCTION_EXPORT(byte_queue_test,"byte_queue test");


#include "mfunc.h"

typedef struct {
    char * name;
    uint8_t old;
} student_t;



unsigned int macro_test(char argc,char ** argv)
{
    student_t skl [10] =
    {
        {"lei",10},
        {"lucy",20},
        {"han",15},
        {"andy",35},
        {"terry",20},
        {"lei",10},
        {"lucy",20},
        {"han",15},
        {"andy",35},
        {"terry",20}
    };
    uint8_t cmd = atoi(argv[1]);

    switch(cmd)
    {
    case 0:
        foreach(student_t,skl,item)
        {
            printf("name %s old %d \r\n",item->name,item->old);
        }
        break;
    case 1:
        using(int i = 100,printf("entry \r\n"),printf("out \r\n"))
        {
            printf("i = %d\r\n",i);
            i++;
            printf("i = %d\r\n",i);
        }
        break;
    }
    return 0;
}

LTSH_FUNCTION_EXPORT(macro_test,"macro test");


unsigned int cortexm(char argc,char ** argv)
{
#define SCB_CPUID_BASE 0xE000ED00U
#define SCB_CPUID      (*((volatile uint32_t *)(SCB_CPUID_BASE)))
#define VARIANT_MASK    0xf
#define VARIANT_SHIFT   20U
#define REVERSION_MASK  0xf
#define REVERSION_SHIFT 0U
#define PART_NO_MASK    0xfff
#define PART_NO_SHIFT   0x04

    uint32_t cpuid = SCB_CPUID;

    ((cpuid & (PART_NO_MASK<<PART_NO_SHIFT)) >> PART_NO_SHIFT) == 0xC20 ? (printf("Cortex-M0:")) :\
    (((cpuid & (PART_NO_MASK<<PART_NO_SHIFT)) >> PART_NO_SHIFT) == 0xC60 ? (printf("Cortex-M0+:")):\
    (((cpuid & (PART_NO_MASK<<PART_NO_SHIFT)) >> PART_NO_SHIFT) == 0xC21 ? (printf("Cortex-M1:")):\
    (((cpuid & (PART_NO_MASK<<PART_NO_SHIFT)) >> PART_NO_SHIFT) == 0xC23 ? (printf("Cortex-M3:")):\
    (((cpuid & (PART_NO_MASK<<PART_NO_SHIFT)) >> PART_NO_SHIFT) == 0xC24) ? (printf("Cortex-M4:")):\
    ((printf("UNKNOWN"))))));

    printf("r%dp%d",((cpuid & (VARIANT_MASK<<VARIANT_SHIFT)) >> VARIANT_SHIFT),\
                    ((cpuid & (REVERSION_MASK<<REVERSION_SHIFT)) >> REVERSION_SHIFT));

    printf("\r\n");

    return 0;
}
LTSH_FUNCTION_EXPORT(cortexm,"show cortexm information");