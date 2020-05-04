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

int16_t main(void) {
    init_elecanisms();

    T1CON = 0x0000;         // set Timer1 to 1024 Hz
    PR1 = 0x3D08;

    TMR1 = 0;               // set Timer1 count to 0
    IFS0bits.T1IF = 0;      // lower Timer1 interrupt flag
    T1CONbits.TON = 1;      // turn on Timer1

    LED2 = ON;

    uint8_t digits[][5] = {
        {7, 5, 5, 5, 7},
        {2, 3, 2, 2, 7},
        {7, 4, 7, 1, 0},
    };


    auto row_pins[] = {D1, D2, D9, D10, D11};
    auto col_pins[] = {D0, D4, D8};

    D0_DIR = OUT;
    D1_DIR = OUT;
    D2_DIR = OUT;
    D4_DIR = OUT;
    D8_DIR = OUT;
    D9_DIR = OUT;
    D10_DIR = OUT;
    D11_DIR = OUT;

    /*
    int i = 0;
    for (i = 0; i < 5; i++) {
        *row_pins[i] = 1;
        __asm__("nop");
    }
    for (i = 0; i < 3; i++) {
        *col_pins[i] = 0;
        __asm__("nop");
    }
    */

    int row = 0, col = 0;

    //row_pins[0] = 1;
    //col_pins[0] = 1;

    int digit = 0;
    while(1) {
        for (row = 0; row < 5; row++) {
            for (col = 0; col < 3; col++) {

                uint8_t this_bit = digits[digit][row] & (1<<col);

                D1 = row != 0;
                __asm__("nop");
                D2 = row != 1;
                __asm__("nop");
                D11 = row != 2;
                __asm__("nop");
                D10 = row != 3;
                __asm__("nop");
                D9 = row != 4;
                __asm__("nop");

                D0 = (col == 0 && this_bit);
                __asm__("nop");
                D4 = (col == 1 && this_bit);
                __asm__("nop");
                D8 = (col == 2 && this_bit);
                __asm__("nop");

                col = (col + 1) % 3;
                if (col == 0) row = (row + 1) % 5;

                // Wait for the timer
                while (IFS0bits.T1IF ==0) {}
                IFS0bits.T1IF = 0;      // lower Timer1 interrupt flag
            }
        }

        // Turn all the LED's off
        D9 = 1;
        __asm__("nop");
        D8 = 0;

        // Do work, like checking buttons and incrementing time
        int i;
        for (i = 0; i < 10000; i++) {
            __asm__("nop");
        }

        //digit = (digit+1) % 3;
    }
}

