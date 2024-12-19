; helper.asm
; Runs on any Cortex M processor
; we align 32 bit variables to 32-bits
; we align op codes to 16 bits

        .thumb

        .data           ; following lines are placed in RAM
        .align  2       ; forces machine code to be halfword-aligned
Data    .space  4       ; reserves 4 bytes in RAM to store the results.

        .text           ; following lines are placed in Flash ROM
        .align 4        ; forces machine code to be word-aligned

        ; pointers to variables
DAddr   .word   Data        ; pointer to Res


        .global Increment
        .global IncrementInit

IncrementInit:  .asmfunc

        ; load pointer registers
        LDR     R0, DAddr
        MOV     R1, #0
        STR     R1, [R0]
        BX      LR
        .endasmfunc

Increment:  .asmfunc

    	; load pointer registers
    	LDR    R1, DAddr
    	LDR    R0, [R1]
        ADD    R0, #1
        MOV    R2, #480
Loop   	SUB    R2, #1
    	CMP    R2, #0
    	BNE    Loop

    	STR    R0, [R1]
    	BX     LR
        .endasmfunc

        .end
