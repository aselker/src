/*
** Copyright (c) 2018, Bradley A. Minch
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met: 
** 
**     1. Redistributions of source code must retain the above copyright 
**        notice, this list of conditions and the following disclaimer. 
**     2. Redistributions in binary form must reproduce the above copyright 
**        notice, this list of conditions and the following disclaimer in the 
**        documentation and/or other materials provided with the distribution. 
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
** CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
** SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
** INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
** CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
** ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
** POSSIBILITY OF SUCH DAMAGE.
*/
#include "elecanisms.h"

void start_32b_timer() {
    // Configure the timer so we know what time it is when we take a reading.
    //
    // We use two 16-bit timers (Timer2 and Timer3) together as a 32-bit timer.
    //
    // I think the system clock runs at 16MHz, so with a 1/256 prescale, each
    // tick will be 16 nanoseconds, and the 32-bit timer will overflow in about
    // a minute.
    //
    // Outputs are TMR3 (high) and TMR2 (low). 65536 per second, I think.

    // This bit is copied from a FRM...
    T2CON = 0x00;          //Stops any 16/32-bit Timer2 operation
    T3CON = 0x00;          //Stops any 16-bit Timer3 operation
    TMR3 = 0x00;           //Clear contents of the timer3 register
    TMR2 = 0x00;           //Clear contents of the timer2 register
    PR3 = 0xFFFF;          //Load the Period register3 with the value 0xFFFF
    PR2 = 0xFFFF;          //Load the Period register2 with the value 0xFFFF


    // Now, configure the timers
    T2CON = 0x8038;
    //      0b1000 0000 0011 1000
    //    Run ^ |        ||| | ^ Use internal timer (Fosc/2)
    //          |        ||| ^ Use as half of a 32-bit timer
    //          |        |^^ 1/256 prescale
    //          |        ^ Gated time accumulation disabled
    //          ^ Continue timer in idle mode

    T3CON = 0x8008;
    //      0b1000 0000 0000 1000
    //    Run ^ |        ||| | ^ Use internal timer (Fosc/2)
    //          |        ||| ^ Use as half of a 32-bit timer
    //          |        |^^ 1/1 prescale -- I think this is correct!
    //          |        ^ Gated time accumulation disabled
    //          ^ Continue timer in idle mode

}

uint32_t time_now() {
    return TMR2 + ((uint32_t)TMR3 << 16);
}

void display(uint8_t rows[5]) {
    int row = 0, col = 0;

    // Wait for the timer
    while (IFS0bits.T1IF ==0) {}
    IFS0bits.T1IF = 0;      // lower Timer1 interrupt flag
    while (IFS0bits.T1IF ==0) {}
    IFS0bits.T1IF = 0;      // lower Timer1 interrupt flag
    for (row = 0; row < 5; row++) {
        switch(row) {
            case 0:
                D9 = 1;
                __asm__("nop");
                D1 = 0;
                break;
            case 1:
                D1 = 1;
                __asm__("nop");
                D2 = 0;
                break;
            case 2:
                D2 = 1;
                __asm__("nop");
                D11 = 0;
                break;
            case 3:
                D11 = 1;
                __asm__("nop");
                D10 = 0;
                break;
            case 4:
                D10 = 1;
                __asm__("nop");
                D9 = 0;
                break;
        }

        D0 = (rows[row] & (1<<0)) >> 0;
        __asm__("nop");
        D4 = (rows[row] & (1<<1)) >> 1;
        __asm__("nop");
        D8 = (rows[row] & (1<<2)) >> 2;
        __asm__("nop");

        // Wait for the timer
        while (IFS0bits.T1IF ==0) {}
        IFS0bits.T1IF = 0;      // lower Timer1 interrupt flag

        D0 = 0;
        __asm__("nop");
        D4 = 0;
        __asm__("nop");
        D8 = 0;
    }

    // Turn all the LED's off
    D9 = 1;
}



int16_t main(void) {
    init_elecanisms();

    T1CON = 0x0000;         // set Timer1 to 1024 Hz
    PR1 = 0x3D08;

    TMR1 = 0;               // set Timer1 count to 0
    IFS0bits.T1IF = 0;      // lower Timer1 interrupt flag
    T1CONbits.TON = 1;      // turn on Timer1

    uint8_t digits[][5] = {
        {7, 5, 5, 5, 7},
        {2, 3, 2, 2, 7},
        {7, 4, 7, 1, 7},
        {7, 4, 7, 4, 7},
        {5, 5, 7, 4, 4},
        {7, 1, 7, 4, 7},
        {7, 1, 7, 5, 7},
        {7, 4, 4, 2, 2},
        {7, 5, 7, 5, 7},
        {7, 5, 7, 4, 7},
        {0, 0, 0, 0, 0},
    };

    D0_DIR = OUT;
    D1_DIR = OUT;
    D2_DIR = OUT;
    D4_DIR = OUT;
    D8_DIR = OUT;
    D9_DIR = OUT;
    D10_DIR = OUT;
    D11_DIR = OUT;




    start_32b_timer();

    int digit = 0;
    int score = 0;
    uint32_t start_time;

    while(1) {

        if (SW1 == 0) {
            start_time = time_now();
            score = -1;
        }

        if (score == -1) {
            digit = (time_now() - start_time) / 65536;
            if (9 < digit) digit = 9;
            if (SW2 == 0) score = digit;
        } else {
            if ((time_now() % 32768) >= 8192) digit = score;
            else digit = 10; // Display nothing
        }

        display(digits[digit]);

    }
}

