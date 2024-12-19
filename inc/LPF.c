// LPF.c
// Runs on MSP432
// implements three FIR low-pass filters

// Jonathan Valvano
// September 12, 2017

/* This example accompanies the book
   "Embedded Systems: Introduction to Robotics,
   Jonathan W. Valvano, ISBN: 9781074544300, copyright (c) 2019
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/

Simplified BSD License (FreeBSD License)
Copyright (c) 2019, Jonathan Valvano, All rights reserved.

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

// Newton's method
// s is an integer
// isqrt(s) is an integer
static uint32_t isqrt(uint32_t s)  {
    uint32_t t = s/10+1;            // initial guess
    for(int n = 16; n; --n) {   // guaranteed to finish
        t = ((t*t + s)/t)/2;
    }
    return t;
}

#define MAX_FILTER_SIZE 512

//************** Digital Low Pass Filter **************

uint32_t FilterSize;        // Size-point average, Size = 1 to 512
uint32_t Queue1[MAX_FILTER_SIZE * 2]; // two copies of multiple access circular queue (MACQ)
uint32_t *Pt1;               // pointer to the current position of MACQ
uint32_t I1;                // index to oldest
uint32_t LPFSum1;            // sum of the last Size samples

void LPF_Init(uint32_t initialValue, uint32_t filterSize) {

    FilterSize = (filterSize > MAX_FILTER_SIZE)? MAX_FILTER_SIZE : filterSize; // max

    I1 = FilterSize-1;
    LPFSum1 = FilterSize*initialValue;  // prime MACQ with initial data
    for(int i = 0; i < 2*FilterSize; i++) {
        Queue1[i] = initialValue;
    }
}


// calculate one filter output, called at sampling rate
// Input: new ADC data   Output: filter output
// y(n) = (x(n)+x(n-1)+...+x(n-FilterSize-1)/FilterSize
uint32_t LPF_Calc(uint32_t newdata) {
    if(I1 == 0) {
        I1 = FilterSize-1;              // wrap
    } else {
        I1--;                     // make room for data
    }
    LPFSum1 = LPFSum1 + newdata - Queue1[I1];   // subtract oldest, add newest
    Queue1[I1] = newdata;     // save new data
    return LPFSum1/FilterSize;
}


// calculate noise as standard deviation, called every time buffer refills
// Input: none   Output: standard deviation
int32_t Noise(void) {

    if(FilterSize < 2) { return 0; }

    int32_t sum = 0;
    for(int i = 0; i < FilterSize; i++) {
        sum = sum + Queue1[i];
    }

    int32_t mean = sum/FilterSize; // DC component

    sum = 0;
    for(int i = 0; i < FilterSize; i++) {
        sum = sum + (Queue1[i]-mean)*(Queue1[i]-mean); // total energy in AC part
    }

    return isqrt(sum/(FilterSize-1));
}



uint32_t Queue2[MAX_FILTER_SIZE * 2];   // two copies of MACQ
uint32_t *Pt2;       // pointer to current
uint32_t LPFSum2;    // sum of the last FilterSize samples

void LPF_Init2(uint32_t initialValue, uint32_t filterSize) {

    FilterSize = (filterSize > MAX_FILTER_SIZE)? MAX_FILTER_SIZE : filterSize;

    Pt2 = &Queue2[0];

    LPFSum2 = FilterSize*initialValue; // prime MACQ with initialValue data

    for(int i = 0; i < 2*FilterSize; i++) {
        Queue2[i] = initialValue;
    }
}


// calculate one filter output, called at sampling rate
// Input: new ADC data   Output: filter output, DAC data
// y(n) = (Queue(n)+Queue(n-1)+...+x(n-FilterSize-1)/FilterSize
uint32_t LPF_Calc2(uint32_t newdata) {
    if(Pt2 == &Queue2[0]) {
        Pt2 = &Queue2[FilterSize-1];              // wrap
    } else {
        Pt2--;                         // make room for data
    }
    LPFSum2 = LPFSum2+newdata -*Pt2;   // subtract oldest, add newest
    *Pt2 = *(Pt2+FilterSize) = newdata;     // two copies of the new data
    return LPFSum2/FilterSize;
}


// calculate noise as standard deviation, called every time buffer refills
// Input: none   Output: standard deviation
int32_t Noise2(void){

    if(FilterSize<2) { return 0; }

    int32_t sum = 0;
    for(int i = 0; i < FilterSize; i++) {
        sum = sum + Queue2[i];
    }

    int32_t mean = sum/FilterSize; // DC component

    sum = 0;
    for(int i = 0; i < FilterSize; i++) {
        sum = sum + (Queue2[i]-mean)*(Queue2[i]-mean); // total energy in AC part
    }

    return isqrt(sum/(FilterSize-1));

}


uint32_t Queue3[MAX_FILTER_SIZE * 2];   // two copies of MACQ
uint32_t *Pt3;       // pointer to current
uint32_t LPFSum3;    // sum of the last FilterSize samples
void LPF_Init3(uint32_t initialValue, uint32_t filterSize) {

    FilterSize = (filterSize > MAX_FILTER_SIZE)? MAX_FILTER_SIZE : filterSize;

    Pt3 = &Queue3[0];

    LPFSum3 = FilterSize*initialValue; // prime MACQ with initial data

    for(int i=0; i<2*FilterSize; i++) {
        Queue3[i] = initialValue;
    }
}


// calculate one filter output, called at sampling rate
// Input: new ADC data   Output: filter output, DAC data
// y(n) = (x(n)+x(n-1)+...+x(n-FilterSize-1)/FilterSize
uint32_t LPF_Calc3(uint32_t newdata) {
    if(Pt3 == &Queue3[0]) {
        Pt3 = &Queue3[FilterSize-1];              // wrap
    } else {
        Pt3--;                         // make room for data
    }
    LPFSum3 = LPFSum3 + newdata - *Pt3;   // subtract oldest, add newest
    *Pt3 = *(Pt3+FilterSize) = newdata;     // two copies of the new data
    return LPFSum3/FilterSize;
}


// calculate noise as standard deviation, called every time buffer refills
// Input: none   Output: standard deviation
int32_t Noise3(void){

    int32_t sum,mean;

    if(FilterSize<2) return 0;
    sum = 0;
    for(int i = 0; i < FilterSize; i++) {
        sum = sum+Queue3[i];
    }
    mean = sum/FilterSize; // DC component
    sum = 0;
    for(int i=0;i<FilterSize;i++) {
        sum = sum+(Queue3[i]-mean)*(Queue3[i]-mean); // total energy in AC part
    }

    return isqrt(sum/(FilterSize-1));
}

