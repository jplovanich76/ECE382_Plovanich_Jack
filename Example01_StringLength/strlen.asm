; strlen.asm
; Runs on any Cortex M processor
; Count the length of a string
; Stan Baek
; May 31, 2022


;Simplified BSD License (FreeBSD License)
;Copyright (c) 2022, Stanley Baek, All rights reserved.
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


; we align 32 bit variables to 32-bits
; we align op codes to 16 bits
        .thumb

        ; With .data, the following lines are placed in "RAM"
        ; A memory space is allocated for the variables in RAM.
        ; They can be modified while the programming is running.
        .data
        .align 2        ; forces machine code to be halfword-aligned
        ; The following variable are place in "RAM" because they are declared under .data.
length  .space 4         ; reserves 4 bytes in "RAM"

        ; With .text, the following lines are placed in "ROM"
        ; The variables in ROM must be initialized with constant values
        ; that cannot be changed while the programming is running.
        .text
        .align 4        ; forces machine code to be word-aligned
        ; The following variable are place in "ROM" because they are declared under .text.
str     .string "This is a string.",0

        ; Pointers to variables
        ; Any data greater than 32 bits (4 bytes) needs a pointer to acess its data.
        ; Addresses must be 32 bits (1 word).
LAddr   .word   length
StrAddr .word   str

        .global  main           ; make the symbol visible to the linker.

main:  .asmfunc                 ; begin assembly main function

        MOV     R2, #0          ; R2 <= 0, length of string.
        LDR     R0, StrAddr     ; RO is the address of str, i.e., a pointer.
Loop    LDRB    R1, [R0], #1    ; A byte of string is loaded onto R1, and increment R0.
        CMP     R1, #0          ; Compare R1 with 0.
        BEQ     Done            ; if R0 == 0 (Null), go to Done
        ADD     R2, #1          ; R2 <= R2 + 1
        B       Loop            ; Go to Loop

Done    LDR     R0, LAddr       ; RO is the address of length, i.e., a pointer.
STR     STR     R2, [R0]        ; Store R2 (string length) to length.

Stall   B       Stall           ; stall here - stay here forever, equivalent to while(1); in C
                                ; Inspect the memory at the address stored in LAddr
       .endasmfunc

       .end










