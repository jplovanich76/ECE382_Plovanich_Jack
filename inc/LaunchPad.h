/**
 * @file      LaunchPad.h
 * @brief     Provide simple I/O functions for the LaunchPad
 * @details   Input from switches, output to LED
 * built-in LED1 connected to P1.0 (not used here)
 * negative logic built-in Button 1 connected to P1.1
 * negative logic built-in Button 2 connected to P1.4
 * built-in red LED connected to P2.0
 * built-in green LED connected to P2.1
 * built-in blue LED connected to P2.2
 * @version   TI-RSLK MAX v1.1
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2019 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      June 28, 2019
<table>
<caption id="LaunchPad">MSP432 LaunchPad</caption>
<tr><th>Pin  <th>Connection
<tr><td>P1.1 <td>negative logic built-in Button 1
<tr><td>P1.4 <td>negative logic built-in Button 2
<tr><td>P2.0 <td>built-in red LED
<tr><td>P2.1 <td>built-in green LED
<tr><td>P2.2 <td>built-in blue LED
<tr><td>P1.0 <td>built-in LED1
</table>
 ******************************************************************************/

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

#ifndef LAUNCHPAD_H_
#define LAUNCHPAD_H_

/*!
 * @defgroup RSLK_Input_Output
 * @brief
 * @{*/


/**
 * \brief RGB Colors
 */
#define RGB_OFF     0x00
#define RED         0x01
#define GREEN       0x02
#define BLUE        0x04

#define YELLOW      0x03  // RED + GREEN
#define MAGENTA     0x05  // RED + BLUE
#define CYAN        0x06  // GREEN + BLUE

#define LED_OFF     0x00
#define LED_FL      0x01
#define LED_FR      0x20
#define LED_BL      0x40
#define LED_BR      0x80

#define SW_OFF      0x00
#define SWR_ON      0x02
#define SWL_ON      0x10

// bit-banding for LEDs
#define LEDOUT (*((volatile uint8_t *)(0x42098040)))
#define REDLED (*((volatile uint8_t *)(0x42098060)))
#define GREENLED (*((volatile uint8_t *)(0x42098064)))
#define BLUELED (*((volatile uint8_t *)(0x42098068)))



/**
 * Initialize LaunchPad I/O
 *
 * @param  none
 * @return none
 * @brief  Initialize Switch inputs and LED outputs
 */
void LaunchPad_Init(void);


/**
 * Input from two switches on LaunchPad
 * Value returned in positive logic
 *
 * @param  none
 * @return two-bit value of switches
 *         0x00 none
 *         0x01 Button1
 *         0x02 Button2
 *         0x03 both Button1 and Button2
 * @brief  Input from Switches
 */
__attribute__((deprecated("Use LaunchPad_SW instead.")))
uint8_t LaunchPad_Input(void);


/**
 * Input from two switches on LaunchPad
 * value returned in positive logic.
 * Use SWR_ON and SWL_ON with this function.
 * For example,
 * if (LaunchPad_SW() == (SWR_ON | SWL_ON)) // if both switches are on
 *
 * @param  none
 * @return two-bit value of switches
 *         0x00 none
 *         0x02 Switch1
 *         0x10 Switch2
 *         0x12 both Switch1 and Switch2
 * @brief  Input from Switches
 */
uint8_t LaunchPad_SW(void);

/**
 * Output to red LED on LaunchPad
 *
 * @param led is value to send to LED,
 * 0 is off; 1 is on
 * @return none
 * @brief  Output to red LED
 */
void LaunchPad_LED(uint8_t led);


/**
 * Output to 3-color LED on LaunchPad
 *
 * @param  led is value to send to LEDs
 * 0 is off; bit0 is red; bit1 is green; bit2 is blue
 * @return none
 * @brief  Output to 3-color LED
 */
void LaunchPad_RGB(uint8_t led);


/**
 * Output to RSLK Chassis LEDs
 *
 * @param led is value to send to LEDs
 * 0 is off;
 * bit0 is front left yellow LED
 * bit5 is front right yellow LED
 * bit6 is back left red LED
 * bit7 is back right red LED
 * @return none
 * @brief  Output to 4 RSLK chassis LEDs
*/
void LaunchPad_RSLK(uint8_t led);


/**
 * Output to 3-color LED on LaunchPad
 *
 * @param  data is value to send to LEDs
 * 0 is off; bit0 is red; bit1 is green; bit2 is blue
 * @return none
 * @brief  Output to 3-color LED
 */
__attribute__((deprecated("Use LaunchPad_RGB instead.")))
void LaunchPad_Output(uint8_t data);


/**
 * Wait until a switch is pressed and release.
 *
 * @param none
 * @return none
 */
void LaunchPad_Wait4SW(void);

#endif /* LAUNCHPAD_H_ */
