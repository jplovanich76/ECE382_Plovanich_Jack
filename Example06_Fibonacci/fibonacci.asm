; power.asm
; Runs on any Cortex M
; Stan Baek
; 11 May 2022
;
;Simplified BSD License (FreeBSD License)
;Copyright (c) 2022, Stan Baek All rights reserved.
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

        .thumb
        .text
        .align 2

        .global  FibASM_1
        .global  FibASM_2

;------------ Fibonacci 1 ------------
; Inputs: R0 = number
; Output: R0 = Fibonacci(R0)
FibASM_1:       .asmfunc
        CMP     R0, #0      ; R0 == 0 ?
        BNE     Fib         ; R0 != 0
        BX      LR          ; Return

        ; need to find fib(n)
Fib     MOV     R2, R0      ; R2 = n
        MOV     R0, #1      ; R0 is the return value
        MOV     R1, #0
Loop    CMP     R2, #1
        BEQ     Done
        MOV     R3, R1      ; R3 is a temporary place.
        MOV     R1, R0
        ADD     R0, R3
        SUB     R2, #1
        B       Loop
Done    BX      LR          ; Return

        .endasmfunc

;------------ Fibonacci 2 ------------
; Inputs: R0 = base, R1 = exponent
; Output: R0 = Power1(R0, R1)
FibASM_2:       .asmfunc
        CMP     R0, #2      ; n >= 2 ?
        BHS     RCS         ; if n >= 2, call itself
        BX      LR          ; return n

RCS     PUSH    {R4, LR}    ; Preserve registers
        MOV     R4, R0      ; R4 = n
        SUB     R0, #1      ; R0 = n-1
        BL      FibASM_2    ; call fib(n-1)
        MOV     R1, R0      ; R1 holds fib(n-1)
        SUB     R0, R4, #2  ; R0 = n-2
        MOV     R4, R1      ; R4 holds fib(n-1)
        BL      FibASM_2    ; call fib(n-2)
        ADD     R0, R4
        POP     {R4, PC}    ; Restore registers and return

        .endasmfunc
