    RSEG    ASM_TEST_CODE:CODE(2)
    thumb

    PUBLIC asm_test_bfi
    PUBLIC asm_test_ror
    PUBLIC asm_test_mov
    PUBLIC asm_test_mvn

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


asm_test_mov:
    movs r0,#1
    mov r0,#0
    mov r1,#1
    movs r0,#0
    movs r1,#1

    bx lr

asm_test_mvn:
    mvns r0,#0          ;r0 = 0xffffffff N=1,Z=0
    mvns r0,#0XFFFFFFFF ;r0 = 0 N=0,Z=1
    mvn  r1,r0
    movt r1,#0xf123     ;MOVT writes a 16-bit immediate value, imm16, to the top halfword, Rd[31:16],
                        ;of its destination register. The write does not affect Rd[15:0].
    movw r1,#0xf123     ;The MOVW instruction provides the same function as MOV, 
                        ;but is restricted to using the imm16operand.

    bx lr

	END
