; cryptography.asm
; Runs on any Cortex M
; Basic XOR encryption and decryption functions for cryptography.
; Capt Steven Beyer
; July 23 2020
; Updated by Stan Baek
; June 03 2021
;
;Simplified BSD License (FreeBSD License)
;Copyright (c) 2020, Steven Beyer, All rights reserved.
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
       .global Encrypt
       .global Decrypt
       .global XOR_bytes

EOM    .byte  '#'


;------------ Encrypt ------------

Encrypt:   .asmfunc
    PUSH {R5, R6, R7, R8, R9, LR}

    ; preserving critical info
    MOV R5, R0         ;R5 -> addy of plain msg
    MOV R6, R1         ;R6 -> key
    MOV R7, R2         ;R7 -> addy to store encrypted msg
    LDRB R9, EOM
EncryptLoop:
    LDRB R8, [R5]      ;R8 -> load plain msg value to R8
    MOV R1, R6
    MOV R0, R8

    BL XOR_bytes       ;XOR
    STRB R0, [R7]      ;R0 -> store encrypted value at R0
    CMP R8, R9         ;check for EOM, then branch if true
    BEQ Done
    ADD R5, R5, #1     ;increments - plain msg (addy)
    ADD R7, R7, #1     ;increments - encrypted msg (addy)
    B EncryptLoop
Done:
    POP {R5, R6, R7, R8, R9, LR}
    BX LR

    .endasmfunc

;---

;------------ Decrypt ------------

Decrypt:    .asmfunc
    PUSH {R5, R6, R7, R8, R9, LR}

    ; preserving critical info
    MOV R5, R0         ;R5 -> addy of encrypted msg
    MOV R6, R1         ;R6 -> key
    MOV R7, R2         ;R7 -> addy of decrypted msg
    LDRB R9, EOM
DecryptLoop:
    LDRB R8, [R5]      ;R8 -> load encrypted value to R8
    MOV R1, R6         ;copy R6 to R1
    MOV R0, R8         ;copy R8 to R0

    BL XOR_bytes       ;XOR
    STRB R0, [R7]      ;R0 -> storing returned decrypted value at R0
    CMP R0, R9         ;check for EOM, then branch if true
    BEQ Done2
    ADD R7, R7, #1     ;increments - decrypted msg (addy)
    ADD R5, R5, #1     ;increments - encrypted msg (addy)
    B DecryptLoop
Done2:
    POP {R5, R6, R7, R8, R9, LR}
    BX LR

    .endasmfunc
    .end

;---
