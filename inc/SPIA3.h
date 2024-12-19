/**
 * @file      SPIA3.h
 * @brief     Provide simple I/O functions for the SPI B1
 * @details   Use eUSCI_B1 to transmit an 8-bit code
 * @version   TI-RSLK MAX v1.1
 * @author    Stan Baek
 * @copyright Copyright 2022 by Stan Baek
  * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      June 08, 2022
 ************************************************************************/

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

#ifndef SPIA3_H_
#define SPIA3_H_

/**
 * Initialize SPI B1. One feature of the
 * MSP432 is that its SSIs can get their baud clock from
 * either the auxiliary clock (ACLK = REFOCLK/1 = 32,768 Hz
 * see <b>Clock.c</b>) or from the low-speed subsystem master
 * clock (SMCLK <= 12 MHz see <b>Clock.c</b>).  The SSI can
 * further divide this clock signal by using the 16-bit Bit
 * Rate Control prescaler Register, UCAxBRW.
 * @param none
 * @return none
 * @note  Assumes low-speed subsystem master clock is 12 MHz
 * @brief  Initialize SPI B1
 */
void SPIA3_Init(void);


/**
 * Wait for transmitter to be empty (let previous frame finish)
 * @param Noe
 * @return None
 * @note Assumes UCA3 and Port 9 have already been initialized and enabled
 */
void SPIA3_Wait4Tx(void);


/**
 * Wait until SPI not busy
 *   Wait if a transmit or receive operation is in progress.
 * @param Noe
 * @return None
 * @note Assumes UCA3 and Port 9 have already been initialized and enabled
 */
void SPIA3_Wait4TxRxReady(void);


/**
 * Write data to TXBUF
 * @param data 8-bit data to transmit
 * @return none
 * @note Assumes UCA3 and Port 9 have already been initialized and enabled
 */
void SPIA3_WriteTxBuffer(char data);


/**
 * @param data character to transmit
 * @return none
 * @brief Transmit a character via SPI A3
 */
void SPIA3_OutChar(char data);


/**
 * @details Transmit a string to EUSCI_A3 SPI
 * @param  ptr is pointer to null-terminated ASCII string to be transferred
 * @return none
 * @note   SPIA3_Init must be called once prior
 * @brief  Transmit string out of MSP432
 */
void SPIA3_OutString(const char* ptr);


#endif /* SPIA3_H_ */
