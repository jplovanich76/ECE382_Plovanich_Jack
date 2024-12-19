// main.c
// Runs on MSP432
// Fibonacci sequence
// Stan Baek
// July 11, 2021

/*
Simplified BSD License (FreeBSD License)
Copyright (c) 2021, Stan Baek, All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are
those of the authors and should not be interpreted as representing official
policies, either expressed or implied, of the FreeBSD Project.
*/


#include <stdint.h>
#include "msp.h"

extern uint32_t FibASM_1(uint32_t);
extern uint32_t FibASM_2(uint32_t);


// 0, 1, 1, 2, 3, 5, 8, 13, 21, 34
// f(0) = 0, f(1) = 1, f(2) = 1, ...
uint32_t Fib1(uint32_t n) {

    if (n == 0) {
        return 0;
    }

    int x = 0;
    int y = 1;
    int temp;

    while (n > 1) {
        temp = x;
        x = y;
        y = temp + y;
        n--;
    }

    return y;
}

uint32_t Fib2(uint32_t n) {

    if (n < 2) {    // n == 0 or n == 1
        return n;
    } else {
        return Fib2(n-1) + Fib2(n-2);
    }
}

void Program1(void) {
    uint32_t f = Fib1(2);
    return;
}

void Program2(void) {
    uint32_t f = Fib2(8);
    return;
}

void Program3(void) {
    uint32_t f = FibASM_1(8);
    return;
}

void Program4(void) {
    uint32_t f = FibASM_2(8);
    return;
}

void main(void) {

    // Program1();
    // Program2();
    // Program3();
    Program4();
}
