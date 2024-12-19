// LaunchPad.c
// Runs on MSP432
// Input from switches, output to LED
// Jonathan Valvano
// February 18, 2017
// Updated by Stan Baek
// May 20, 2023

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
 
// built-in LED1 connected to P1.0
// negative logic built-in Button 1 connected to P1.1
// negative logic built-in Button 2 connected to P1.4
// built-in red LED connected to P2.0
// built-in green LED connected to P2.1
// built-in blue LED connected to P2.2

#include "msp.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"

//------------LaunchPad_Init------------
// Initialize Switch input and LED output
// Input: none
// Output: none
void LaunchPad_Init(void){

    // P1.0: LED
    // P1.1: Switch 1
    // P1.4: Switch 2
    P1->SEL0 &= ~0x13;
    P1->SEL1 &= ~0x13;    // 1) configure P1.4, P1.1, and P1.0 as GPIO
    P1->DIR &= ~0x12;     // 2) make P1.4 and P1.1 in
    P1->DIR |= 0x01;      //    make P1.0 out
    P1->REN |= 0x12;      // 3) enable pull resistors on P1.4 and P1.1
    P1->OUT |= 0x12;      //    P1.4 and P1.1 are pull-up

    // P2.0-P2.2: RGB LED
    P2->SEL0 &= ~0x07;
    P2->SEL1 &= ~0x07;    // 1) configure P2.2-P2.0 as GPIO
    P2->DIR |= 0x07;      // 2) make P2.2-P2.0 out
    P2->DS |= 0x07;       // 3) activate increased drive strength
    P2->OUT &= ~0x07;     //    all LEDs off

    // P8.0 & P8.5-7: Chassis LEDs
    P8->SEL0 &= ~0xE1;
    P8->SEL1 &= ~0xE1;    // 1) configure P8.0 and P8.5-7 as GPIO
    P8->DIR |= 0xE1;      // 2) make P2.2-P2.0 out
    P8->OUT &= ~0xE1;     //    all LEDs off
}

//------------ LaunchPad_SW ------------
// Input from Switches 
// Input: none
// Output: 0x00 none
//         0x02 Switch1 (SWR)
//         0x10 Switch2 (SWL)
//         0x12 both Switch1 and Switch2
uint8_t LaunchPad_SW(void){
    return ~(P1->IN) & 0x12;   // read P1.4, P1.1 inputs
}


//------------LaunchPad_LED------------
// Output to LaunchPad red LED
// Input: 0 off, 1 on
// Output: none
void LaunchPad_LED(uint8_t led){  // write one bit to P1.0
    P1->OUT = (P1->OUT & ~0x01) | (led & 0x01);
}


//------------LaunchPad_Output------------
// Output to LaunchPad LEDs 
// Input: 0 off, bit0=red, bit1=green, bit2=blue
// Output: none
void LaunchPad_RGB(uint8_t led){  // write three outputs bits of P2
    P2->OUT =  (P2->OUT & ~0x07) | (led & 0x07);
}



//------------LaunchPad_Output------------
// Output to RSLK Chassis LEDs
// Input: P8.0: FL Yellow, P8.5: FR Yellow, P8.6: BL Red, P8.7: BR Red
// Output: none
void LaunchPad_RSLK(uint8_t led){  // write three outputs bits of P2
    P8->OUT = (P8->OUT & ~0xE1) | (led & 0xE1);
}


// Wait until a switch is pressed and release.
void LaunchPad_Wait4SW(void) {
    // We need to wait for release, touch, and release to
    // prevent multiple readings.

    // wait for release
    while(LaunchPad_SW() != SW_OFF) {
        Clock_Delay1ms(200); LaunchPad_RGB(RGB_OFF); // off
        Clock_Delay1ms(200); LaunchPad_RGB(YELLOW); // yellow
    }

    // wait for touch
    while(LaunchPad_SW() == SW_OFF) {
        Clock_Delay1ms(100); LaunchPad_RGB(RGB_OFF); // off
        Clock_Delay1ms(100); LaunchPad_RGB(MAGENTA); // magenta
    }

    // wait for release
    while(LaunchPad_SW() != SW_OFF) {
        Clock_Delay1ms(100); LaunchPad_RGB(RGB_OFF); // off
        Clock_Delay1ms(100); LaunchPad_RGB(CYAN); // cyan
    }
}


// ============== Deprecated functions =============

//------------LaunchPad_Input------------
// Input from Switches
// Input: none
// Output: 0x00 none
//         0x01 Button1
//         0x02 Button2
//         0x03 both Button1 and Button2
uint8_t LaunchPad_Input(void){
    return ((((~(P1->IN))&0x10)>>3)|(((~(P1->IN))&0x02)>>1));   // read P1.4, P1.1 inputs
}

//------------LaunchPad_Output------------
// Output to LaunchPad LEDs
// Input: 0 off, bit0=red, bit1=green, bit2=blue
// Output: none
void LaunchPad_Output(uint8_t data){  // write three outputs bits of P2
    P2->OUT = (P2->OUT&0xF8)|data;
}






