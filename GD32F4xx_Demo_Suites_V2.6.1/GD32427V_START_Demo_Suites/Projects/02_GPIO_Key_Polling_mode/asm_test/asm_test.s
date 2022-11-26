    RSEG    ASM_TEST_CODE:CODE(2)
    thumb

    PUBLIC asm_test_bfi
    PUBLIC asm_test_ror

asm_test_bfi:
    ldr r0,=0xaabbccff
    ldr r1,=0x00000000
    ldr r2,=0x00000000
    bfi r1,r0,#0,#8 /* copy r0 bit0-7 to r2 bit0-7 */
    bfi r2,r0,#1,#10 /* cpoy r0 bit0-9 t0 r2 bit1-bit10 */
    
    bx lr
    
/*Rotate Right (immediate) provides the value of the contents of a register rotated by a constant value. The bits that
are rotated off the right end are inserted into the vacated bit positions on the left. It can optionally update the
condition flags based on the result.
*/
asm_test_ror:
    ldr r0,=0xaabbccdd
    ldr r1,=0x00000000
    ldr r2,=0x00000000
    ror r1,r0,#16 /* r1 = 0xccddaabb */
    ror r2,r0,#24 /* r2 = 0xbbccddaa */
    
    bx lr    

	END

