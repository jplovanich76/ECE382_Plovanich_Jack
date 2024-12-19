/**
 * @file      Nokia5110.h
 * @brief     Provide simple I/O functions for the Nokia 5110
 * @details   Use eUSCI_A3 to send an 8-bit code to the Nokia5110 48x84
 * pixel LCD to display text, images, or other information.
 * @version   TI-RSLK MAX v1.1
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2019 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      June 28, 2019
 * @note derived from Nathan Seidle
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/<br><br>
 * Font table, initialization, and other functions based
 * off of Nokia_5110_Example from Spark Fun:<br>
 * 7-17-2011<br>
 * Spark Fun Electronics 2011<br>
 * Nathan Seidle<br>
 * http://dlnmh9ip6v2uc.cloudfront.net/datasheets/LCD/Monochrome/Nokia_5110_Example.pde
 * @warning      Set the CONTRAST from 0xA0 (lighter) to 0xCF (darker) for your particular display
 * @note Hardware connections
<table>
<caption id="Nokia5110_ports">Nokia 5110 LCD</caption>
<tr><th>Pin   <th>Nokia 5110 function
<tr><td>+3.3 V<td>VCC
<tr><td>ground<td>GND
<tr><td>P9.4  <td>SCE, active low enable
<tr><td>P9.3  <td>RST, reset=0; run=1
<tr><td>P9.6  <td>D/C, data=1; command=0
<tr><td>P9.7  <td>DN(MOSI), master out slave in
<tr><td>P9.5  <td>SCLK, serial clock
<tr><td>+3.3 V<td>LED, optional LED backlight
</table>

<table>
<caption id="Blue Adafruit 338 Nokia 5110">Nokia 5110 Adafruit 338 LCD</caption>
<tr><th>Signal        <th>(Nokia 5110) LaunchPad pin
<tr><td>Ground        <td>(Gnd, pin 1) ground
<tr><td>3.3V          <td>(Vcc, pin 2) power
<tr><td>UCA3CLK       <td>(Clk, pin 3) connected to P9.5
<tr><td>UCA3SIMO      <td>(Din, pin 4) connected to P9.7
<tr><td>Data/Command  <td>(D/C, pin 5) connected to P9.6
<tr><td>UCA3STE       <td>(CS,  pin 6) connected to P9.4
<tr><td>Reset         <td>(RST, pin 7) connected to P9.3
<tr><td>back light    <td>(LED, pin 8) not connected, consists of 4 white LEDs which draw ~80mA total
</table>
 ******************************************************************************/

/* This example accompanies the book
   "Embedded Systems: Introduction to Robotics,
   Jonathan W. Valvano, ISBN: 9781074544300, copyright (c) 2019
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/

Simplified BSD License (FreeBSD License)
Copyright (c) 2017, Jonathan Valvano, All rights reserved.

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

// Blue Nokia 5110
// ---------------
// Signal        (Nokia 5110) LaunchPad pin
// Reset         (RST, pin 1) connected to P9.3
// UCA3STE       (CE,  pin 2) connected to P9.4
// Data/Command  (DC,  pin 3) connected to P9.6
// UCA3SIMO      (Din, pin 4) connected to P9.7
// UCA3CLK       (Clk, pin 5) connected to P9.5
// 3.3V          (Vcc, pin 6) power
// back light    (BL,  pin 7) not connected, consists of 4 3.3 V white LEDs which draw ~80mA total
// Ground        (Gnd, pin 8) ground

// Red SparkFun Nokia 5110 (LCD-10168)
// -----------------------------------
// Signal        (Nokia 5110) LaunchPad pin
// 3.3V          (VCC, pin 1) power
// Ground        (GND, pin 2) ground
// UCA3STE       (SCE, pin 3) connected to P9.4
// Reset         (RST, pin 4) connected to P9.3
// Data/Command  (D/C, pin 5) connected to P9.6
// UCA3SIMO      (DN,  pin 6) connected to P9.7
// UCA3CLK       (SCLK, pin 7) connected to P9.5
// back light    (LED, pin 8) not connected, consists of 4 3.3 V white LEDs which draw ~80mA total


#ifndef NOKIA5110_H_
#define NOKIA5110_H_

extern const uint8_t af_logo[];

// Adjust this from 0xA0 (lighter) to 0xCF (darker) for your display.
// Start from 0xB1;
// try 0xB1 (for 3.3V red SparkFun),
// 0xB8 (for 3.3V blue SparkFun),
// 0xBF if your display is too dark, or 0x80 to 0xFF if experimenting

void Nokia5110_SetContrast(uint8_t contrast);


/**
 * Initialize Nokia 5110 48x84 LCD by sending the proper
 * commands to the PCD8544 driver.  One feature of the
 * MSP432 is that its SSIs can get their baud clock from
 * either the auxiliary clock (ACLK = REFOCLK/1 = 32,768 Hz
 * see <b>Clock.c</b>) or from the low-speed subsystem master
 * clock (SMCLK <= 12 MHz see <b>Clock.c</b>).  The SSI can
 * further divide this clock signal by using the 16-bit Bit
 * Rate Control prescaler Register, UCAxBRW.
 * @param none
 * @return none
 * @note  Assumes low-speed subsystem master clock is 12 MHz
 * @see Nokia5110_OutChar(), Nokia5110_Clear(), Nokia5110_PrintBMP()
 * @brief  Initialize LCD driver
 */
void Nokia5110_Init(void);


/**
 * Print a character to the Nokia 5110 48x84 LCD.  The
 * character will be printed at the current cursor position,
 * the cursor will automatically be updated, and it will
 * wrap to the next row or back to the top if necessary.
 * One blank column of pixels will be printed on either side
 * of the character for readability.  Since characters are 8
 * pixels tall and 5 pixels wide, 12 characters fit per row,
 * and there are six rows.
 * @param data  character to print
 * @return none
 * @note  LCD is in default horizontal addressing mode (V = 0)
 * @see Nokia5110_Init(), Nokia5110_Clear(), Nokia5110_SetCursor()
 * @brief  Print a character to the LCD
 * @note use Nokia5110_SetCursor to specify position
 */
void Nokia5110_OutChar(char data);


/**
 * Print a string of characters to the Nokia 5110 48x84 LCD.
 * @param ptr  pointer to NULL-terminated ASCII string
 * @return none
 * @note  LCD is in default horizontal addressing mode (V = 0)
 * @see Nokia5110_Init(), Nokia5110_Clear(), Nokia5110_SetCursor()
 * @brief  Print a string of characters to the LCD
 * @note use Nokia5110_SetCursor to specify position
 */
void Nokia5110_OutString(const char * ptr);


/**
 * Print a number in unsigned decimal format to the Nokia LCD.
 * @param n  32-bit unsigned number
 * @param min_length minimum length to print.
 * If min_length is less than or equal to the length of the string,
 * min_length will be ignored. If it is greater than the length of the string,
 * spaces will be added in front of the string.
 * @return none
 * @note  LCD is in default horizontal addressing mode (V = 0)
 * @see Nokia5110_Init(), Nokia5110_Clear(), Nokia5110_SetCursor()
 * @brief  Print a 32-bit unsigned number to the LCD
 * @note If the integer is 12345 and min_length = 7,
 * it will print "  12345".
 * Notice that two spaces have been added to the front.
 * If the integer is 12345 and min_length <= 5, it will print "12345".
 *
 * Note: You can simply set min_length = 0 if you like to print the actual length of the string.
 */
void Nokia5110_OutUDec(uint32_t n, int min_length);

/**
 * Print a number in signed decimal format to the Nokia LCD.
 * @param n  32-bit unsigned number
 * @param min_length minimum length to print.
 * If min_length is less than or equal to the length of the string,
 * min_length will be ignored. If it is greater than the length of the string,
 * spaces will be added in front of the string.
 * @return none
 * @note  LCD is in default horizontal addressing mode (V = 0)
 * @see Nokia5110_Init(), Nokia5110_Clear(), Nokia5110_SetCursor()
 * @brief  Print a 32-bit unsigned number to the LCD
 * @note If the integer is -12345 and min_length = 7,
 * it will print " -12345".
 * Notice that a space has been added to the front.
 * If the integer is -12345 and min_length <= 6, it will print "-12345".
 * If the integer is 12345 and min_length <=6, it will print " 12345".
 * Notice there is a space at the beginning, which makes consistent with
 * Nokia5110_OutS32Dec, Nokia5110_OutS16Dec, and Nokia5110_OutS8Dec.
 *
 * Note: You can simply set min_length = 0 if you like to print the actual length of the string.
 */
void Nokia5110_OutSDec(int32_t n, int min_length);


/**
 * Output two hex digits to the Nokia 5110 LCD.
 * Prints 4 characters with leading 0x
 * @param n unsigned number 0-255 to print
 * @return none
 * @brief  Print two hex digits
 * @note use Nokia5110_SetCursor to specify position
 */
void Nokia5110_OutU8Hex(uint8_t n);

/**
 * Output 8 bit number the Nokia 5110 LCD.
 * Prints 8 characters with leading b
 * @param n unsigned number 0-255 to print
 * @return none
 * @brief  Print 8 bit number
 * @note use Nokia5110_SetCursor to specify position
 */
void Nokia5110_Out8Bin(uint8_t n);


/**
 * Move the cursor to the desired row and column position.
 * The next character will be printed here.
 * row = 1 is the top row and row = 6 is the bottom row
 * col = 1 is the left most column and col = 12 is the right most column.
 * @param row row position of the cursor (1 <= row <= 12)
 * @param col column position of the cursor (1 <= col <= 12)
 * @return none
 * @see Nokia5110_Init(), Nokia5110_Clear(), Nokia5110_SetCursor()
 * @brief  Set coordinates of next print
 * @note Use Nokia5110_SetCursor2 to specify position
 */
void Nokia5110_SetCursor2(uint8_t row, uint8_t col);


/**
 * NOTE: This function is deprecated. Use Nokia5110_SetCursor2 instead.
 *
 * Move the cursor to the desired X- and Y-position.  The
 * next character will be printed here.  X=0 is the leftmost
 * column.  Y=0 is the top row.
 * @param newX  new X-position of the cursor (0<=newX<=11)
 * @param newY  new Y-position of the cursor (0<=newY<=5)
 * @return none
 * @see Nokia5110_Init(), Nokia5110_Clear(), Nokia5110_SetCursor()
 * @brief  Set coordinates of next print
 * @note use Nokia5110_SetCursor to specify position
 */
__attribute__((deprecated("Use Nokia5110_SetCursor2 instead.")))
void Nokia5110_SetCursor(uint8_t newX, uint8_t newY);


/**
 * Clear the LCD by writing zeros to the entire screen and
 * reset the cursor to (0,0) (top left corner of screen).
 * @param none
 * @return none
 * @see Nokia5110_Init(), Nokia5110_OutChar(), Nokia5110_SetCursor()
 * @brief  Clear the screen and reset cursor
 * @note use Nokia5110_SetCursor to specify position
 */
void Nokia5110_Clear(void);

/**
 * Fill the whole screen by drawing a 48x84 bitmap image.
 * @param ptr  pointer to 504 byte bitmap
 * @return none
 * @note  LCD is in default horizontal addressing mode (V = 0)
 * @see Nokia5110_Init(), Nokia5110_OutChar(), Nokia5110_Clear()
 * @brief  Draw 48x84 formatted bitmap image.
 */
void Nokia5110_DrawFullImage(const uint8_t *ptr);

/**
 * Bitmaps defined above were created for the LM3S1968 or
 * LM3S8962's 4-bit grayscale OLED display.  They also
 * still contain their header data and may contain padding
 * to preserve 4-byte alignment.  This function takes a
 * bitmap in the previously described format and puts its
 * image data in the proper location in the buffer so the
 * image will appear on the screen after the next call to
 *   Nokia5110_DisplayBuffer();
 * The interface and operation of this process is modeled
 * after RIT128x96x4_BMP(x, y, image);
 * @param  xpos      horizontal position of bottom left corner of image, columns from the left edge<br>
 *                     must be less than 84<br>
 *                     0 is on the left; 82 is near the right
 * @param  ypos      vertical position of bottom left corner of image, rows from the top edge<br>
 *                     must be less than 48<br>
 *                     2 is near the top; 47 is at the bottom
 * @param  ptr       pointer to a 16 color BMP image
 * @param  threshold grayscale colors above this number make corresponding pixel 'on'<br>
 *                     0 to 14<br>
 *                     0 is fine for ships, explosions, projectiles, and bunkers
 * @return none
 * @note Call Nokia5110_DisplayBuffer() to see this bitmap.
 * @see Nokia5110_ClearBuffer(), Nokia5110_DisplayBuffer(), Nokia5110_SetPxl()
 * @brief  Put 48x84 unformatted 4-bit color bitmap image in internal screen buffer.
 */
void Nokia5110_PrintBMP(uint8_t xpos, uint8_t ypos, const uint8_t *ptr, uint8_t threshold);

/**
 * There is a buffer in RAM that holds one screen.  This
 * routine clears that buffer.
 * @param none
 * @return none
 * @see Nokia5110_PrintBMP(), Nokia5110_DisplayBuffer(), Nokia5110_SetPxl()
 * @brief  Clear internal screen buffer.
 */
void Nokia5110_ClearBuffer(void);

/**
 * Fill the whole screen by drawing a 48x84 screen image
 * from the RAM buffer.
 * @param none
 * @return none
 * @note  LCD is in default horizontal addressing mode (V = 0)
 * @see Nokia5110_PrintBMP(), Nokia5110_ClearBuffer(), Nokia5110_SetPxl()
 * @brief  Draw internal screen buffer to the display.
 */
void Nokia5110_DisplayBuffer(void);

/**
 * Clear the internal screen buffer pixel at (i, j),
 * turning it off.
 * @param i  the row index  (0 to 47 in this case),    y-coordinate
 * @param j  the column index  (0 to 83 in this case), x-coordinate
 * @return none
 * @note Call Nokia5110_DisplayBuffer() to see this change.
 * @see Nokia5110_ClearBuffer(), Nokia5110_DisplayBuffer(), Nokia5110_SetPxl()
 * @brief  Turn a pixel off in internal screen buffer.
 */
void Nokia5110_ClrPxl(uint32_t i, uint32_t j);

/**
 * Set the internal screen buffer pixel at (i, j),
 * turning it on.
 * @param i  the row index  (0 to 47 in this case),    y-coordinate
 * @param j  the column index  (0 to 83 in this case), x-coordinate
 * @return none
 * @note Call Nokia5110_DisplayBuffer() to see this change.
 * @see Nokia5110_ClearBuffer(), Nokia5110_DisplayBuffer(), Nokia5110_ClrPxl()
 * @brief  Turn a pixel on in internal screen buffer.
 */
void Nokia5110_SetPxl(uint32_t i, uint32_t j);


#endif /* NOKIA5110_H_ */
