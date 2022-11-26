    RSEG    ASM_TEST_CODE:CODE(2)
    thumb

    PUBLIC asm_test_bfi

asm_test_bfi:
    ldr r0,=0xaabbccff
    ldr r1,=0x00000000
    ldr r2,=0x00000000
    bfi r1,r0,#0,#8
    bfi r2,r0,#1,#10
    
    bx lr

	END

