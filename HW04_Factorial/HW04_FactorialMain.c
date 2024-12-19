// main.c
// Runs on MSP432
// Determine whether a number is prime or composite.
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
#include "../inc/Clock.h"
#include "../inc/CortexM.h"


/*
 * fact_iterative
 * Finds the factorial of a non-negative integer, n!
 * input: non-negative integer
 * output: n!
 * Note: It returns 1 if n = 0, i.e., 0! = 1
 */
uint32_t fact_iterative(uint8_t n) {

    uint32_t ret = 1;

    // n * (n-1) * (n-2) * ... * 1
    while (n != 0) {
        ret = ret * n;
        n--;
    }

    return ret;
}


/*
 * fact_recursive
 * Finds the factorial of a non-negative integer, n!
 * input: non-negative integer
 * output: n!
 * Note: It returns 1 if n = 0, i.e., 0! = 1
 */
uint32_t fact_recursive(uint8_t n) {

    if (n == 0) {   // exit condition of recursion.
        return 1;
    } else {
        return n * fact_recursive(n-1); // n * (n-1)!
    }
}

#define SIZE    5

/*
 * main function
 * Call fact_iterative and fact_recursive functions
 */
void main(void) {

    uint8_t numbers[SIZE] = {1, 2, 4, 7, 0}; // non-negative integers to test.

    uint32_t result1[SIZE];  // result of iterative method
    uint32_t result2[SIZE];  // Result of recursive method

    // Use the iterative function for each number
    for (int i = 0; i < SIZE; i++) {
        result1[i] = fact_iterative(numbers[i]);
    }

    // Use the recursive function for each number
    for (int i = 0; i < SIZE; i++) {
        result2[i] = fact_recursive(numbers[i]);
    }

     while(1);   // Stay here forever and observe the results in Expressions tab
}


