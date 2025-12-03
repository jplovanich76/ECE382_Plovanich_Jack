#include <stdint.h>

#include <stdbool.h>

#include "msp.h"

#include "../inc/Clock.h"

#include "../inc/CortexM.h"

#include "../inc/LaunchPad.h"

#include "../inc/Motor.h"

#include "../inc/Nokia5110.h"

#include "../inc/Tachometer.h"

#include "../inc/TimerA2.h"

#include "../inc/TA3InputCapture.h"

#include "../inc/Bump.h"

#include "../inc/UART0.h"

#include "../inc/TimerA1.h"

#include "../inc/Clock.h"           // System clock management

#include "../inc/CortexM.h"         // Cortex M specific functions

#include "../inc/PWM.h"             // PWM signal control

#include "../inc/Motor.h"           // Motor control

#include "../inc/LPF.h"             // Low-pass filter

#define N 0

#define W 1

#define S 2

#define E 3

// =============== Program 16.1 =====================================

int16_t  LeftDistance_mm;   // Distance traveled by the left motor (in mm)

int16_t  RightDistance_mm;  // Distance traveled by the right motor (in mm)

#define BUFFER_SIZE 3000    // Size of data buffers for storing measurements

#define BUMP_DELAY 1000

uint16_t CoordBufferX[BUFFER_SIZE];

uint16_t CoordBufferY[BUFFER_SIZE];


static void TxBuffer(void) {

    // wait for release

    while(LaunchPad_SW() || Bump_Read()) {

        Clock_Delay1ms(200); LaunchPad_RGB(RGB_OFF); // off

        Clock_Delay1ms(200); LaunchPad_RGB(RED); // red

    }

    uint8_t isTxEnabled = 0;

    while(1) {

        Nokia5110_Clear();

        Nokia5110_OutString("Tx Buffer?");

        Nokia5110_SetCursor2(2,1);  Nokia5110_OutString("Bump: Y/N");

        Nokia5110_SetCursor2(4,1);  Nokia5110_OutString("SW to Cont.");

        while(!LaunchPad_SW()) {    // until SW is pressed

            // update the screen

            if (isTxEnabled) {      // Display Y/N on LCD

                Nokia5110_SetCursor2(1,12);

                Nokia5110_OutString("Y");

            } else {

                Nokia5110_SetCursor2(1,12);

                Nokia5110_OutString("N");

            }

            if (Bump_Read()) {

                isTxEnabled ^= 1;   // Toggle Y/N

            }

            // flash the blue LED while desired speeds are updated.

            BLUELED ^= 1;

            Clock_Delay1ms(200);

        }

        // SW is pressed

        if(isTxEnabled) {   // Tx Buffer? is Y. Start transmitting data via USB

            UART0_OutString("\n\r***Receiving buffer data***\n\r");

            for (int i = 0; i < BUFFER_SIZE; i++) {

                UART0_OutUDec(i); UART0_OutChar(',');

                UART0_OutSDec(CoordBufferX[i]); UART0_OutChar(',');

                UART0_OutSDec(CoordBufferY[i]); UART0_OutString("\n\r");

            }

            Nokia5110_SetCursor2(4,1);

            Nokia5110_OutString("TX is Done   ");

            Nokia5110_SetCursor2(5,1);

            Nokia5110_OutString("SW to Cont.");

            while(!LaunchPad_SW());     // until SW is pressed

            Clock_Delay1ms(500);

        } else {    // Tx Buffer? is N. wrap up the function and exit.

            break;

        }

    }

    for(int k = 0; k < 5; k++){

        LaunchPad_RGB(YELLOW);

        Clock_Delay1ms(100);

        LaunchPad_RGB(RGB_OFF);

        Clock_Delay1ms(100);

    }

}


// =============== Program 16.3 =====================================

// Develop a finite state machine (FSM) to control the robot's movement.

// The robot moves forward, backward, and makes left or right turns based on bump sensor feedback.

uint16_t head = N;

int32_t x = 0;

int32_t y = 0;

int32_t oldX = 0;

int32_t oldY = 0;

uint16_t homeY = 0;

uint16_t homeX = 360;

uint16_t numLTurns = 0;

int32_t oldAhLeft = 0;

int32_t oldAhRight = 0;

int32_t ErrorX = 0;

int32_t ErrorY = 0;

//I took out right turn/left turn and made each degree turn a different state for simplicity

typedef enum {

    Stop = 0,      // Stop state

    Forward,       // Move forward

    Backward,      // Move backward

    Left90,        //90 deg left

} state_t;

static state_t CurrentState = Forward;  // Initial state: Forward

static state_t NextState = Forward;     // Next state variable


// Structure to define robot movement commands

typedef struct command {

    uint16_t left_permil;     // PWM duty cycle for the left wheel

    uint16_t right_permil;    // PWM duty cycle for the right wheel

    void (*MotorFunction)(uint16_t, uint16_t);  // Motor function to call (e.g., Motor_Forward)

    int32_t dist_mm;          // Wheel displacement in mm

} command_t;

// Control parameters for various states (distances and duty cycles)

#define FRWD_DIST       1000000000   // forward distance

#define BKWD_DIST       90   // backward distance

#define TR90_DIST       93   // 90 degree turn

#define NUM_STATES      4

// Control commands for each state

command_t ControlCommands[NUM_STATES] = {

    {0,   0,   &Motor_Stop,        0},          // Stop indefinitely

    {380, 375, &Motor_Forward,     FRWD_DIST},  // Move forward until bump sensor triggered

    {150, 150, &Motor_Backward,    BKWD_DIST},  // Move backward for 90mm

    {150, 150, &Motor_TurnLeft,    TR90_DIST},          // Turn left, 90

};

// Clear the LCD and display initial state

static void LCDClear3(void) {

    Nokia5110_Clear();                  // Clear the entire display

    Nokia5110_OutString("SECT. 8 BOT"); // Bot name

    Nokia5110_SetCursor2(3,1); Nokia5110_OutString("X");  // Show current state

    Nokia5110_SetCursor2(4,1); Nokia5110_OutString("Y");   // Show distance traveled

    Nokia5110_SetCursor2(5,1); Nokia5110_OutString("D");

}

// Update the LCD with the current state and motor data

static void LCDOut3(void) {

    Nokia5110_SetCursor2(3, 3);         //display x-coordinate

    Nokia5110_OutSDec(x, 5);

    Nokia5110_SetCursor2(4, 3);         //display y-coordinate

    Nokia5110_OutSDec(y, 5);

    Nokia5110_SetCursor2(5, 3);         //display orientation (number corresponds to direction, as defined)

    Nokia5110_OutSDec(head, 1);

}

//based on heading, update coordinates

static void Coordinates(uint16_t heading, uint32_t distance) {

    if (heading == N) {

        y = y + distance;

    }

    else if (heading == W) {

        x = x + distance;

    }

    else if (heading == S) {

        y = y - distance;

    }

    else {

        x = x - distance;

    }

}

//toggles LEDs

// This function is called every 1ms by TimerA2.

void Blink(void) {

    if (ControlCommands[CurrentState].left_permil == 0) {

        static uint16_t Time_1ms = 0;

        Time_1ms++;

        if (Time_1ms < 5) {

            LaunchPad_RGB(RED);

        }

        else if (Time_1ms <= 9) {

            LaunchPad_RGB(BLUE);

        }

        // if it increments to 10, roll over to 0.

        else if (Time_1ms >= 10) {

            Time_1ms = 0;

        }

        else {

        }

    }

    else {

        LaunchPad_RGB(RED);

    }

}


// Counter for how many times the controller function has been called

static uint8_t NumControllerExecuted = 0;  // Updated every 20ms

static void UpdateParameters(void) {

    uint32_t delay_ms = BUMP_DELAY;   // Delay between bump checks

    // Step 1: Wait for bump or launchpad button release.

    while(LaunchPad_SW() || Bump_Read()) {

        Clock_Delay1ms(delay_ms); LaunchPad_RGB(RGB_OFF); // Red LED off

        Clock_Delay1ms(delay_ms); LaunchPad_RGB(RED);     // Red LED on

    }

    // Display initial update menu on Nokia LCD screen.

    Nokia5110_Clear();

    Nokia5110_SetCursor2(1,1); Nokia5110_OutString("Coords Data");

    Nokia5110_SetCursor2(6,1);  Nokia5110_OutString("SW to Cont.");  // Instruct to press SW to continue

        // Flash blue LED to indicate parameter update.

        BLUELED ^= 1;

        Clock_Delay1ms(delay_ms);

    // Step 3: Wait for SW or Bump button release to avoid accidental inputs.

    while(LaunchPad_SW() || Bump_Read()) {

        Clock_Delay1ms(200); LaunchPad_RGB(RGB_OFF); // Red LED off

        Clock_Delay1ms(200); LaunchPad_RGB(RED);     // Red LED on

    }

    Nokia5110_Clear();

    Nokia5110_SetCursor2(6,1);  Nokia5110_OutString("SW to Cont.");  // Instruct to press SW to continue

        // Toggle blue LED during parameter adjustment.

        BLUELED ^= 1;

        Clock_Delay1ms(delay_ms);


    // Step 6: Flash yellow LED to signal completion of parameter update.

    for(int k = 0; k < 5; k++){

        LaunchPad_RGB(YELLOW);

        Clock_Delay1ms(100);

        LaunchPad_RGB(RGB_OFF);

        Clock_Delay1ms(100);

    }

}

// Main control logic for the robot, executed by the TimerA2 ISR every 20ms

static void Controller3(void) {

    static uint16_t timer_20ms = 0;   // Timer to track elapsed time

    static uint8_t bumpRead = 0x00;   // Stores the bump sensor reading

    NumControllerExecuted++;  // Increment counter every time the controller runs

    // Get the PWM duty cycles for the current state

    uint16_t left_permil = ControlCommands[CurrentState].left_permil;

    uint16_t right_permil = ControlCommands[CurrentState].right_permil;

    // FSM Output: Execute the motor command for the current state

    ControlCommands[CurrentState].MotorFunction(left_permil, right_permil);

    // State transition logic based on bump sensors and distance

    bumpRead = Bump_Read();  // Read bump sensor status

    Tachometer_GetDistances(&LeftDistance_mm, &RightDistance_mm);  // Get current wheel distances


    //after getting tacho data, establish current data, declare vars for storing (eventual) old data

    int32_t currentLeft = LeftDistance_mm;

    int32_t currentRight = RightDistance_mm;

    //differentiate - since we update every 20ms, we need to compare to previous data to update coords

    int32_t dL = currentLeft - oldAhLeft;

    int32_t dR = currentRight - oldAhRight;

    int32_t dT = (dR + dL) / 2;

    //current data becomes old, sets up next comparison

    oldAhLeft = LeftDistance_mm;

    oldAhRight = RightDistance_mm;

    switch (CurrentState) {

        case Stop:  // Remain in Stop state indefinitely, LED blink - TimerA1

            break;

        case Forward:  // Moving forward

            if (LeftDistance_mm >= ControlCommands[CurrentState].dist_mm) {

                NextState = Stop;

            }

            else if (bumpRead & 0x04){

                NextState = Backward;

            }

            else if (bumpRead & 0x08){

                NextState = Backward;

            }

            break;

        case Backward:  // Moving backward

            if (LeftDistance_mm <= -(ControlCommands[CurrentState].dist_mm)) {          //basically functions as abs to properly compare values

                NextState = Left90;

            }

            break;

        case Left90:                //syntax for addressing multiple cases - apparently cannot do case Left30 | Left60 | Left 90

            if (RightDistance_mm >= ControlCommands[CurrentState].dist_mm) {

                numLTurns += 1;

                head = numLTurns % 4;

                NextState = Forward;

            }

            break;

        default:

            break;

    }


    Coordinates(head, dT); //coordinates, heading

    ErrorX = homeX - x;

    ErrorY = homeY - y;

    if ((ErrorX <= 150) && (ErrorX >= -150) && (ErrorY <= 150) && (ErrorY >= -150)) {  //home check

        NextState = Stop;   //if home, stop - one of these works

        CurrentState = Stop;

    }

    // Update the timer or reset if transitioning to a new state

    if (CurrentState == NextState) {

        timer_20ms++;  // Stay in current state, increment timer

    } else {

        timer_20ms = 0;  // New state, reset timer and distance measurements

        Tachometer_ResetSteps();

        oldAhRight = 0;

        oldAhLeft = 0;

    }

    // Set the current state to the next state for the next iteration

    CurrentState = NextState;

}

// ========== Main Program: Finite State Machine Control ==========

void Program1(void) {

    // ========== Initialization Phase ==========

    DisableInterrupts();    // Disable interrupts during initialization

    Clock_Init48MHz();      // Set the system clock to 48 MHz

    LaunchPad_Init();       // Initialize the LaunchPad hardware (buttons, LEDs)

    Bump_Init();            // Initialize bump sensors

    Motor_Init();           // Initialize motor driver

    Nokia5110_Init();       // Initialize Nokia 5110 LCD display

    Tachometer_Init();      // Initialize tachometers for wheel distance measurement

    uint32_t const period_2us = 50000;      //2 * 50000 = 100000

    TimerA1_Init(&Blink, period_2us);

    // Set LCD contrast

    uint8_t const contrast = 0xA8;

    Nokia5110_SetContrast(contrast);  // Adjust LCD contrast

    // Set TimerA2 to call the Controller3() function every 20 ms (50 Hz)

    const uint16_t period_4us = 5000;       // 20 ms period

    TimerA2_Init(&Controller3, period_4us); // Initialize TimerA2

    Tachometer_ResetSteps();  // Reset tachometer distance measurements

    // Set the LCD update rate to 10 Hz (update every 5 controller cycles)

    const uint16_t LcdUpdateRate = 5;    // 50 Hz / 5 = 10 Hz

    LCDClear3();  // Clear the LCD and display initial state

    uint32_t const baudrate = 115200; // Set UART baud rate for PC communication

    UART0_Init(baudrate);             // Initialize UART0 communication with set baud rate

    EnableInterrupts();  // Enable interrupts

    // ========== Main Loop ==========

    while(1) {

        // Enter low-power mode while waiting for the next interrupt

        WaitForInterrupt();

        // Update the LCD every 10 Hz

        if (NumControllerExecuted == LcdUpdateRate) {

            LCDOut3();  // Output current state and motor data to the LCD

            NumControllerExecuted = 0;  // Reset the execution count

        if (ControlCommands[CurrentState].dist_mm == 0) {

            TxBuffer();

        }

        }

    }

}


void main(void){

    Program1();

}



