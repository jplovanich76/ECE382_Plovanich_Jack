// BumpInt.c
// Runs on MSP432, interrupt version
// Provide low-level functions that interface bump switches on the robot.
// Daniel Valvano and Jonathan Valvano
// July 11, 2019

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

// Negative logic bump sensors
// P4.7 Bump6, left side of robot
// P4.6 Bump5
// P4.5 Bump4
// P4.3 Bump3
// P4.2 Bump2
// P4.0 Bump1, right side of robot

#include <stdint.h>
#include "msp.h"


void (*BumpTask)(uint8_t bumpSensor);   // user function

// Initialize Bump sensors
// Make six Port 4 pins inputs
// Activate interface pullup
// pins 7,6,5,3,2,0
// Interrupt on falling edge (on touch)
void BumpInt_Init(void(*task)(uint8_t)){

    // write this as part of Lab 14
	BumpTask = task;
	// 1. Configure P4.7-P4.5, P4.3, P4.2, and P4.0 as GPIO
	P4->SEL0 &= ~0xED;         //0b11101101, clear
	P4->SEL1 &= ~0xED;         //clear


	// 2. Make P4.7-P4.5, P4.3, P4.2, and P4.0 input
	P4->DIR &= ~0xED;          //clear

	// 3. Enable pull-up resistors on P4.7-P4.5, P4.3, P4.2, and P4.0
	P4->REN |= 0xED;           //set
	P4->OUT |= 0xED;           //set

	// 4. P4.7-P4.5, P4.3, P4.2, and P4.0 are falling edge event
	//    Set to falling edge events PxIFG flag is set with a high-to-low transition
	P4->IES  |= 0xED;

	// 5. Clear all flags (IFG -> no interrupts pending)
	P4->IFG &= ~0xED;

    // 6. Enable interrupt on P4.7-P4.5, P4.3, P4.2, and P4.0 (enable IE)
	P4->IE |= 0xED;            //set

	// 7. Setup the Nested Vector Interrupt Controller with priority 1
	//    Ensure you choose the correct NVIC
    //    Enable IRQ 38 in NVIC
	NVIC->IP[38] = (1 << 5);                        //enable IRQ 38, shift bits to align with only 7:5 being used
    NVIC->ISER[1] = (1 << 6);                       //set bit 6 in ISR[1]

}


// Read current state of 6 switches
// Returns a 6-bit positive logic result (0 to 63)
// bit 5 Bump6
// bit 4 Bump5
// bit 3 Bump4
// bit 2 Bump3
// bit 1 Bump2
// bit 0 Bump1
uint8_t BumpInt_Read(void){
    // 1. Read the sensors (which are active low) and convert to active high
    uint8_t sensors = (~P4->IN);

    // 2. Select, shift, combine, and output
    uint8_t sevenToFive = ((sensors & 0xE0) >> 2);
    uint8_t twoToThree = ((sensors & 0x0C) >> 1);
    uint8_t zero = (sensors & 0x01);
    uint8_t out = (sevenToFive | twoToThree | zero);

    return out;
}


// we do not care about critical section/race conditions
// triggered on touch, falling edge
void PORT4_IRQHandler(void){
    // clear interrupt flag (No interrupt is pending))
    P4->IFG &= ~0xED;
    // Call Bump_Read()
    (*BumpTask)(BumpInt_Read());
}
