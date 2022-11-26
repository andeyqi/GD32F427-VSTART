#include "asm_test.h"
#include "utilities.h"
#include "littleshell.h"

static unsigned int test[4] = {0xa5a5a5a5};

unsigned int asmtest(char argc,char ** argv)
{
    int cmd  = 0;

    if(argc == 1 )
        return 0;
    
    cmd = myatoi(argv[1]);
    switch(cmd)
    {
    case 0:
        mytest(test);
        break;
    case 1:
        asm_test_bfi();
        break;
    case 2:
        asm_test_ror();
        break;
    default:
        break;
    }
    
    return 1;
}
LTSH_FUNCTION_EXPORT(asmtest,"dump addr value");

