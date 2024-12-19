; Ex02_Addressing.asm
; Runs on any Cortex M processor
; A very simple first project implementing a random number generator
; Daniel Valvano, May 12, 2015
; Updated by Stan Baek, May 20, 2022

; This example accompanies the book
;   "Embedded Systems: Introduction to Robotics,
;   Jonathan W. Valvano, ISBN: 9781074544300, copyright (c) 2019
; For more information about my classes, my research, and my books, see
; http://users.ece.utexas.edu/~valvano/
;
;Simplified BSD License (FreeBSD License)
;Copyright (c) 2019, Jonathan Valvano, All rights reserved.
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


; CAREFULLY READ EVERY COMMENT TO ENSURE WHAT EACH LINE IS DOING!
; we align 32 bit variables to 32-bits
; we align op codes to 16 bits
        .thumb

        ; With .data, the following lines are placed in "RAM"
        ; A memory space is allocated for the variables in RAM.
        ; They can be modified while the programming is running.
        .data
        .align 2        ; forces data to be halfword-aligned
        ; The following variable are place in "RAM" because they are declared under .data.
result  .space 32       ; reserves 32 bytes in "RAM"
msg     .word 0x0ECE0382, 0xFeedBabe, 0x0Defaced, 0x0AFA0C26; msg is 4 x 1 word (4 bytes) = 16 bytes

        ; With .text, the following lines are placed in "ROM"
        ; The variables in ROM must be initialized with constant values
        ; that cannot be changed while the programming is running.
        .text
        .align 4        ; forces machine code to be word-aligned
        ; The following variable are place in "ROM" because they are declared under .text.
count   .word 10        ; count is a 4 byte (1 word) constant variable, and its value is 10.
str     .string "This is a string",0  ; string with a NULL (0) character at the end.

        ; Pointers to variables
        ; Anything greater than 32 bits (4 bytes) needs a pointer to acess its data.
        ; Addresses must be 32 bits (1 word).
ResAddr .word   result
MsgAddr .word   msg
StrAddr .word   str

        .global  main       ; make the symbol visible to the linker.

main:   .asmfunc             ; begin assembly main function

        MOV R0, #100        ; R0 <= 100
        MOV R1, R0          ; R1 <= R0
        ADD R1, #2          ; R1 <= R1 + 2
        ADD R2, R0, R1      ; R2 <= R0 + R1, Observe the value of R2

        LDR R0, MsgAddr     ; Load R0 with the address of msg, making R0 a pointer to msg.
                            ; Observe the value of R0.
                            ; Enter the value of R0 into Memory Browser to inspect the memory.
        LDR R1, [R0]        ; Observe the values of R0 & R1
        LDR R2, [R0, #4]    ; Observe the values of R0 & R2
        LDR R3, [R0, #4]    ; Observe the values of R0 & R3
        LDR R4, [R0]        ; Observe the values of R0 & R4
        LDR R5, [R0], #4    ; Observe the values of R0 & R5
        LDR R6, [R0], #1    ; Observe the values of R0 & R6
        LDR R7, [R0], #4    ; Observe the values of R0 & R7
        LDR R8, [R0]        ; Observe the values of R0 & R8

        LDR R0, count       ; R0 <= 10
        MOV R1, #0          ; R1 <= 0

Loop1   CMP R0, #0          ; Observe PSR after executing this instruction.
        BLE StrCpy          ; if R0 <= 0, go to StrCpy
        ADD R1, R0          ; R1 <= R1 + R0
        ADD R0, #-1         ; R0 <= R0 + (-1)
        B   Loop1           ; Go to Loop1

StrCpy  LDR R0, StrAddr     ; Load R0 with the address of str, making R0 a pointer to str.
        LDR R1, ResAddr     ; Load R1 with the address of str, making R1 a pointer to str.

Loop2   LDRB R2, [R0], #1   ; Observe the values of R0 & R2
        STRB R2, [R1], #1   ; Inspect the memory at the address stored in R1.
        CMP  R2, #0         ; Observe PSR after executing this instruction.
        BNE  Loop2          ; Go to Loop2 if R2 != 0, else move to next instuction, B Stall

Stall   B   Stall           ; stall here - stay here forever, equivalent to while(1); in C
                            ; Inspect the memory at the address stored in ResAddr
       .endasmfunc

       .end




