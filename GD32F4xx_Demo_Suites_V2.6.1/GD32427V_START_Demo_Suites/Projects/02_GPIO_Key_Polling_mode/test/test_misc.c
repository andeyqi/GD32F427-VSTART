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