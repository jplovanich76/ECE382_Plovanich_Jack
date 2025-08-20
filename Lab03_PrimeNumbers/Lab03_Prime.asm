; prime.asm
; Runs on any Cortex M
; Dr. Stan Baek, July 23 2021
;
; 2024-08-25: Update the comment

;Simplified BSD License (FreeBSD License)
;Copyright (c) 2021, Stan Baek, All rights reserved.
;
;Redistribution and use in source and binary forms, with or without modification,
;are permitted provided that the following conditions are met:
;
;1. Redistributions of source code must retain the above copyright notice,
;   this list of conditions and the following disclaimer.
;2. Redistributions in binary form must reproduce the above copyright notice,
;   this list of conditions and the following disclaimer in the documentation
;   and/or other materials provided with the distribution.
;
;THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
;AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
;IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
;ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
;LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
;DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
;LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
;AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
;OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
;USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;
;The views and conclusions contained in the software and documentation are
;those of the authors and should not be interpreted as representing official
;policies, either expressed or implied, of the FreeBSD Project.

;------------Data Alignment and Storage------------
; - Aligns 32-bit variables to 32-bit boundaries and instructions to 16-bit boundaries
; - The `.thumb` directive tells the assembler that we're using the Thumb instruction set
.thumb

; ------------Data Section------------
        .data       ; The following data will be placed in RAM
        .align 2    ; Ensures data is halfword-aligned (2-byte alignment)

Data    .space 10   ; Reserves 10 bytes in RAM (not currently used)
Result  .space 16   ; Reserves 16 bytes in RAM for storing results of primality tests
                    ; Each result is stored as 1 byte (8 bits)
                    ; 1 represents a prime number, -1 represents a composite number
                    ; Since we need to store results for 16 numbers, we reserve 16 bytes

; ------------Text Section------------
        .text       ; The following code will be placed in Flash ROM
        .align 4    ; Ensures code is word-aligned (4-byte alignment)

; ------------Data Arrays------------
Nums    .word 1, 2, 7, 10, 15, 62, 97, 282, 408, 467, 967, 6687, 25913, 25481, 118061, 0
                    ; Nums: Array of positive integers (unsigned 32-bit values)
                    ; These numbers will be tested to determine if they are prime or composite
                    ; The value 0 marks the end of the array

; ------------Pointers to Data------------
ResAddr  .word  Result  ; Stores the address of the Result array
NumsAddr .word  Nums    ; Stores the address of the Nums array


        .global  main   ; Declares main as a global function

main:   .asmfunc        ; Begins the assembly function

; ------------Initialization------------
        LDR     R2, ResAddr     ; Load the address of the Result array into R2 (pointer to store results)
        LDR     R3, NumsAddr    ; Load the address of the Nums array into R3 (pointer to read numbers)

Loop1:
        LDR     R0, [R3], #4    ; Load a number from Nums into R0 and increment R3 to the next number
        CMP     R0, #0          ; Check if the current number is 0 (end of array)
        BEQ     Stall           ; If 0, branch to Stall (end of the program)

        CMP     R0, #1          ; Check if the current number is 1
        BEQ     False           ; If 1, branch to False (1 is not a prime number)

        ASR     R5, R0, #1      ; Divide the number by 2 (m = n / 2) and store the result in R5
        MOV     R6, #2          ; Initialize R6 with 2 (i = 2)

Loop2:
; ============ Add your code below ==============================
; solution

		CMP   R6, R5 		    ; Compare i with m (i > n / 2?)
		BHI   True				; If i > m, the number is prime (branch to True)
                                ; UNSIGNED NUMBERS - not negative

; ============= End of your code ================================

                                ; Check if the number is divisible by i
        UDIV    R8, R0, R6      ; Divide the number by i (R0 / R6) and store the result in R8
        MUL     R9, R8, R6      ; Multiply the quotient by i and store the result in R9
        CMP     R0, R9          ; Compare the original number with the product (n == int(n / i) * i?)

; ============ Add your code below ==============================

; solution
    ;if (r0 == r9)
        ;branch false
    ;else
        ;increment i
        ; branch loop 2

    CMP     R0, R9
    BEQ     False
    ADD     R6, #1
    BNE     Loop2

True:
    MOV     R11, #1
    STRB    R11, [R2], #1
    ADD     R6, #1
    B       Loop1

False:
    MOV     R12, #0
    STRB    R12, [R2], #1
    B       Loop1


; ============= End of your code ================================


; do not modify anything below this line
Stall   B       Stall       ; Stay here and observe the registers and memory

       .endasmfunc
       .end
