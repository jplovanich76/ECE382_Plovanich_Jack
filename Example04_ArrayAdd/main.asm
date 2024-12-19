; Array_Add.asm
; by George York
;    let array Y equal array X plus 3
; Aug 20 2020

        .thumb
        .data
        .align  2
Y_array .space  32              ; create 32 bytes in RAM to hold the output Y array

        .text
        .align  4
X_array .byte   1, 2, 3, 4, 5, 6, 7, 8, 9, 10       ; initialize X_array to 10 numbers in ROM
N       .byte   10                                  ; N is the length of the array

YAddr   .word   Y_array         ; get address or pointer to Y_array
XAddr   .word   X_array         ; get address or pointer to X_array
three   .word   3

        .global main

main:   .asmfunc

        ; load pointer registers
        LDR     R0, YAddr
        LDR     R1, XAddr
        ; load constant number N
        LDRB    R3, N
        ; load constant number "3"
        LDR     R6, three

        ; Loop for the length of the array, letting Y = 3 + X
loop    LDRB    R4, [R1], #1    ; powerful instruction. loads the byte R1 is pointing to into R4, and then increments R1 to point in next byte in array
        ADD     R5, R4, R6      ; let y = x + 3
        STRB    R5, [R0], #1    ; stores the byte in R5 into RAM where R0 is pointing, and then increments R0 to point in next byte in array

        ; let's downcount R3 by one until it is zero to know when we are done
        SUB     R3, R3, #1
        CMP     R3, #0
        BNE     loop            ; if not done, loop back to do next byte, else quit

Stall   B       Stall           ; stall here and observe the registers

        .endasmfunc

        .end
