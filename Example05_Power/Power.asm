; Power.asm
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

        .global  PowerASM_1
        .global  PowerASM_2

;------------ PowerASM_1, Iterative ------------
; Inputs: base, exponent
; Output: base to the power of exponent
PowerASM_1:     .asmfunc

        MOV     R2, R0          ; R2 = base; Move R0 to R2;
        MOV     R0, #1          ; R0 = 1; R0 should be used for return value.
Loop    CMP     R1, #0          ; exp == 0 ?
        BEQ     Done            ; If so, done.
        MUL     R0, R0, R2      ; R0 = R0*R2 (ret = ret * base)
        SUB     R1, R1, #1      ; R1-- (exp--)
        B       Loop            ; Go to Loop
Done    BX      LR              ; Return

        .endasmfunc

;------------ PowerASM_2, Recursive ------------
; Inputs: base, exponent
; Output: base to the power of exponent
PowerASM_2:     .asmfunc
        CMP     R1, #0          ; exp == 0 ?
        BNE     RCS             ; if not, go to RCS
        MOV     R0, #1          ; R0 = 1
        BX      LR              ; if exp == 0, return 1

RCS     PUSH    {LR, R4}        ; Preserve registers
        MOV     R4, R0          ; Save R0 (n) into R4 for later us
        SUB     R1, #1          ; R0 = n-1 (exp-1)
        BL      PowerASM_2      ; Call power(base, exp-1)
        MUL     R0, R0, R4      ; R0 = base * power(base, exp-1)
        POP     {LR, R4}        ; Restore registers
        BX      LR              ; Return

        .endasmfunc

