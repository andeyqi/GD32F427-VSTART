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



unsigned int args(char argc,char ** argv)
{

    print_num(5,1,2,3,4,5);

    return 1;
}
LTSH_FUNCTION_EXPORT(args,"test args");