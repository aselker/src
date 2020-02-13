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
#include <math.h>
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

WORD32 pwm_temp; // I think this is for type-punning between word and unsigned long

int16_t main(void) {

    init_elecanisms();


    // Set direction pin
    D6_DIR = OUT;
    D6 = 1; 

    // Enable the driver
    D7_DIR = OUT;
    D7 = 0;

    // Set PWM pin (it'll get overwritten by PWM hardware)
    D5_DIR = OUT;
    D5 = 0;


    // Set motor controller 1 to be on 100%, for testing
    D8_DIR = OUT;
    D8 = 1;
    D9_DIR = OUT;
    D9 = 1;
    D10_DIR = OUT;
    D10 = 0;


    __builtin_write_OSCCONL(OSCCON & 0xBF);
    ((uint8_t *)&RPOR0)[D5_RP] = OC1_RP;  // connect the OC1 module output to pin D5
    __builtin_write_OSCCONL(OSCCON | 0x40);

    /*
     * We use Edge-Aligned PWM Mode, which is a lot like Continuous Pulse
     * Mode.  The timer runs freely, and the output goes high when it hits
     * OC1R, and then low when it hits OC1RS.
     */

    OC1CON1 = 0x1C06;   // configure OC1 module to use the peripheral 
                        //   clock (i.e., FCY, OCTSEL<2:0> = 0b111) and 
                        //   and to operate in edge-aligned PWM mode
                        //   (OCM<2:0> = 0b110)
    OC1CON2 = 0x001F;   // configure OC1 module to syncrhonize to itself
                        //   (i.e., OCTRIG = 0 and SYNCSEL<4:0> = 0b11111)
    
    OC1RS = (uint16_t)(FCY / 30e3 - 1.);     // configure period register to 
                                            //   get a frequency of 1kHz
    OC1R = OC1RS >> 2;  // configure duty cycle to 1/4
    OC1TMR = 0;         // set OC1 timer count to 0

    start_32b_timer(); // Start the 32-bit timer


    float duty_cycle;

    while(1) {
        float duty_cycle = (1 + sin(TMR2 * 6.2832 / 65536)) / 2;

        OC1R = OC1RS * duty_cycle;

    }

}
