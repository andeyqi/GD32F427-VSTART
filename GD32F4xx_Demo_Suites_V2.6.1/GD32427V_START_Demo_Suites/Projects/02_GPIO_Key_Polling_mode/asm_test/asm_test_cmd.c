#include "asm_test.h"
#include "utilities.h"
#include "littleshell.h"


unsigned int asmtest(char argc,char ** argv)
{
    int cmd  = 0;

    if(argc == 1 )
        return 0;
    
    cmd = myatoi(argv[1]);
    switch(cmd)
    {
    case 1:
        asm_test_bfi();
        break;
    case 2:
        asm_test_ror();
        break;
    case 3:
        asm_test_mov();
        break;
    default:
        break;
    }
    
    return 1;
}
LTSH_FUNCTION_EXPORT(asmtest,"dump addr value");

