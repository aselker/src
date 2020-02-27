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
#include "ajuart.h"
#include "elecanisms.h"
#include "usb.h"
#include <stdio.h>

// I don't know how many digits we'll need so let's use all of 'em
#define TAU 6.283185307179586476925286766559005768394338798750211641949



#define CURRENT_LIMIT 3000
#define DUTY_CYCLE_DEAD_SPOT 130
#define DUTY_CYCLE_LIMIT 32767 // Out of 32768


#define RES_VAL 0.075
#define VREF 3.3
float scaling = VREF/1024;

#define ENC_MISO            D1
#define ENC_MOSI            D0
#define ENC_SCK             D2
#define ENC_CSn             D3

#define ENC_MISO_DIR        D1_DIR
#define ENC_MOSI_DIR        D0_DIR
#define ENC_SCK_DIR         D2_DIR
#define ENC_CSn_DIR         D3_DIR

#define ENC_MISO_RP         D1_RP
#define ENC_MOSI_RP         D0_RP
#define ENC_SCK_RP          D2_RP

#define LED_OVERCURRENT     LED1 // Red
#define LED_OVER_DUTY_CYCLE LED2 // Green

#define CMD_CYCLE_MODE 0
#define CMD_GET_MODE 1
#define CMD_CONST_SLIDER 5
#define CMD_SPRING_SLIDER 6
#define CMD_DAMPER_SLIDER 7
#define CMD_WALL_SLIDER 8
#define CMD_BUMPS_SLIDER 9

#define MODE_CONST 0
#define MODE_SPRING 1
#define MODE_DAMPER 2
#define MODE_WALL 3
#define MODE_BUMPS 4

uint16_t mode = MODE_CONST;
int16_t const_slider = 0, spring_slider = 0, damper_slider = 0, wall_slider = 0, bumps_slider = 0;

// Constants for current PID TODO: init here
int16_t gain_p, gain_i, gain_d;

// Global variables, because we have to persistently track the encoder's
// position
int16_t encoder_revolutions = 0; // How many revolutions have we done?
uint16_t encoder_last_reading = 0;   // Last reading (0-16383)



void vendor_requests(void) {
    switch (USB_setup.bRequest) {
        case CMD_CYCLE_MODE:
            mode = (mode + 1) % 5;
            encoder_revolutions = 0;
            BD[EP0IN].bytecount = 0;
            BD[EP0IN].status = UOWN | DTS | DTSEN;
            break;

        case CMD_GET_MODE:
            BD[EP0IN].address[0] = mode;
            BD[EP0IN].bytecount = 1;
            BD[EP0IN].status = UOWN | DTS | DTSEN;
            break;

        case CMD_CONST_SLIDER:
            const_slider = (int16_t)USB_setup.wValue.w;
            BD[EP0IN].bytecount = 0;
            BD[EP0IN].status = UOWN | DTS | DTSEN;
            break;

        case CMD_SPRING_SLIDER:
            spring_slider = (int16_t)USB_setup.wValue.w;
            BD[EP0IN].bytecount = 0;
            BD[EP0IN].status = UOWN | DTS | DTSEN;
            break;

        case CMD_DAMPER_SLIDER:
            damper_slider = (int16_t)USB_setup.wValue.w;
            BD[EP0IN].bytecount = 0;
            BD[EP0IN].status = UOWN | DTS | DTSEN;
            break;

        case CMD_WALL_SLIDER:
            wall_slider = (int16_t)USB_setup.wValue.w;
            BD[EP0IN].bytecount = 0;
            BD[EP0IN].status = UOWN | DTS | DTSEN;
            break;

        case CMD_BUMPS_SLIDER:
            bumps_slider = (int16_t)USB_setup.wValue.w;
            BD[EP0IN].bytecount = 0;
            BD[EP0IN].status = UOWN | DTS | DTSEN;
            break;
    }

}

void start_pwm() {
    // Set up PWM on pin D5
    // Duty cycle is configured by setting OC1R to some fraction of OC1RS

    __builtin_write_OSCCONL(OSCCON & 0xBF);
    ((uint8_t *)&RPOR0)[D8_RP] = OC1_RP;  // connect the OC1 module output to driver 2's PWM pin
    //((uint8_t *)&RPOR0)[D10_RP] = OC1_RP;  // connect the OC1 module output to driver 2's enable pin
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
    /*OC1R = OC1RS >> 2;  // configure duty cycle to 1/4*/
    OC1R = 0;           // Set duty cycle to 0
    OC1TMR = 0;         // set OC1 timer count to 0
}

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

void setup_encoder() {
    uint8_t *RPOR, *RPINR;
    // Configure encoder pins and connect them to SPI2
    ENC_CSn_DIR = OUT; ENC_CSn = 1;
    ENC_SCK_DIR = OUT; ENC_SCK = 0;
    ENC_MOSI_DIR = OUT; ENC_MOSI = 0;
    ENC_MISO_DIR = IN;

    RPOR = (uint8_t *)&RPOR0;
    RPINR = (uint8_t *)&RPINR0;

    __builtin_write_OSCCONL(OSCCON & 0xBF);
    RPINR[MISO2_RP] = ENC_MISO_RP;
    RPOR[ENC_MOSI_RP] = MOSI2_RP;
    RPOR[ENC_SCK_RP] = SCK2OUT_RP;
    __builtin_write_OSCCONL(OSCCON | 0x40);

    SPI2CON1 = 0x003B;              // SPI2 mode = 1, SCK freq = 8 MHz
    SPI2CON2 = 0;
    SPI2STAT = 0x8000;
}

uint16_t even_parity(uint16_t v) {
    v ^= v >> 8;
    v ^= v >> 4;
    v ^= v >> 2;
    v ^= v >> 1;
    return v & 1;
}

WORD read_encoder(WORD address) {

    /*WORD address = 0x3FFF; // I think we can hard-code this*/

    WORD cmd, result;
    uint16_t temp;

    cmd.w = 0x4000 | address.w;         // set 2nd MSB to 1 for a read
    cmd.w |= even_parity(cmd.w) << 15;

    ENC_CSn = 0;

    SPI2BUF = (uint16_t)cmd.b[1];
    while (SPI2STATbits.SPIRBF == 0) {}
    temp = SPI2BUF;

    SPI2BUF = (uint16_t)cmd.b[0];
    while (SPI2STATbits.SPIRBF == 0) {}
    temp = SPI2BUF;

    ENC_CSn = 1;

    __asm__("nop");     // p.12 of the AS5048 datasheet specifies a minimum
    __asm__("nop");     //   high time of CSn between transmission of 350ns
    __asm__("nop");     //   which is 5.6 Tcy, so do nothing for 6 Tcy.
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");

    ENC_CSn = 0;

    SPI2BUF = 0;
    while (SPI2STATbits.SPIRBF == 0) {}
    result.b[1] = (uint8_t)SPI2BUF;

    SPI2BUF = 0;
    while (SPI2STATbits.SPIRBF == 0) {}
    result.b[0] = (uint8_t)SPI2BUF;

    ENC_CSn = 1;

    return result; // Remember to & 0x3FFF before using angle
}

int16_t get_current(unsigned char motor){
    char sensepin = A2_AN;
    if(motor){
        sensepin = A3_AN;
    }
    return (read_analog(sensepin)-512) * 16;
    //2.2 amps max measurement
    //233 units/amp
    //0.0043 amps/unit
}

float get_current_amps(unsigned char motor){
    char sensepin = A2_AN;
    if(motor){
        sensepin = A3_AN;
    }
    float measuredvoltage = (float)read_analog(sensepin)*scaling;
    float vdrop = (measuredvoltage - VREF/2)/10;
    float current = vdrop/RES_VAL; //note this will be negative depending on direction
    return current;
}

int32_t last_current_error = 0;
int32_t integral = 0;
unsigned char is_overcurrent = 0;
unsigned char is_over_duty_cycle = 0;

void current_pid_reset() {
    last_current_error = 0;
    integral = 0;
}

int32_t current_pid_tick(int32_t target) {
    if (CURRENT_LIMIT < target) {
        target = CURRENT_LIMIT;
        is_overcurrent = 1;
        LED_OVERCURRENT = 1;
    } else if (target < -CURRENT_LIMIT) {
        target = -CURRENT_LIMIT;
        is_overcurrent = 1;
        LED_OVERCURRENT = 1;
    } else {
        is_overcurrent = 0;
        LED_OVERCURRENT = 0;
    }


    int32_t current_error = get_current(1) - target;

    int32_t proportional = current_error;
    int32_t derivative = (current_error - last_current_error);
    // Don't update integral if we're over duty cycle and the signs are the same
    if (!is_over_duty_cycle || ((integral < 0) ^ (current_error < 0))) integral += current_error;

    int32_t sum = -((int32_t)gain_p*proportional/1024 + (int32_t)gain_i*integral/1024 - (int32_t)gain_d*derivative/1024);

    last_current_error = current_error;

    return sum;
}


int32_t get_encoder_pos() {
    uint16_t reading = read_encoder((WORD)0x3FFF).w & 0x3FFF;

    if ((encoder_last_reading < 4096) && (12288 < reading)) {
        encoder_revolutions -= 1;
    }

    if ((reading < 4096) && (12288 < encoder_last_reading)) {
        encoder_revolutions += 1;
    }

    encoder_last_reading = reading;

    return ((int32_t)16384 * encoder_revolutions) + reading;
}

void set_duty_cycle(unsigned char motor, int32_t duty_cycle) {
    // Takes a number from -32767 to 32767, sets duty cycle such that 32768 is
    // full forward
    // Clips to +- DUTY_CYCLE_LIMIT


    if(duty_cycle < 0) {
        if (motor) D9 = 1;
        else D6 = 1;
        duty_cycle = -duty_cycle;
    } else {
        if (motor) D9 = 0;
        else D6 = 0;
    }

    // Dead-spot compensation
    duty_cycle += DUTY_CYCLE_DEAD_SPOT;

    if (DUTY_CYCLE_LIMIT < duty_cycle) {
        duty_cycle = DUTY_CYCLE_LIMIT;
        is_over_duty_cycle = 1;
        LED_OVER_DUTY_CYCLE = 1;
    } else {
        is_over_duty_cycle = 0;
        LED_OVER_DUTY_CYCLE = 0;
    }

    // For 'pwm' pin
    OC1R = (uint16_t)(((uint32_t)OC1RS * duty_cycle) / 32768);

    // For 'enable' pin
    //OC1R = (uint16_t)(((uint32_t)OC1RS * (32768 - duty_cycle)) / 32768);
}


int32_t duty_cycle;
int32_t encoder_pos;
int32_t last_encoder_pos = 0;
int32_t speed;
int32_t current_goal;
int32_t sawtooth;
int32_t wall_pos;

// Interrupt Service Routine (ISR)
void __attribute__((interrupt, auto_psv)) _T1Interrupt(void) {
    D13 = 1;
    D12 = !D12;
    IFS0bits.T1IF = 0;      // lower Timer1 interrupt flag

    encoder_pos = get_encoder_pos();

    // PWM spring mode
    //duty_cycle = encoder_pos / 32;

    switch(mode) {
        case MODE_CONST:
            current_goal = 50 * const_slider;
            break;
        case MODE_SPRING:
            // Spring mode
            current_goal = encoder_pos * spring_slider / 400;
            break;
        case MODE_DAMPER:
            // Damper mode
            speed = encoder_pos - last_encoder_pos;
            last_encoder_pos = encoder_pos;
            current_goal = speed * abs(speed) * damper_slider / 24;
            break;
        case MODE_WALL:
            speed = encoder_pos - last_encoder_pos;
            last_encoder_pos = encoder_pos;

            wall_pos = (int32_t)wall_slider*1024;

            current_goal = ((encoder_pos - wall_pos) / 4) + (speed * abs(speed) / 2);

            if(wall_slider > 0){
                if (encoder_pos > wall_pos) {
                    current_goal = 0;
                }
            } else{
                if (encoder_pos < wall_pos) {
                    current_goal = 0;
                }
              }


            break;

        case MODE_BUMPS:
            if (encoder_pos<0) encoder_pos = -encoder_pos;
            sawtooth = (encoder_pos % ((int32_t)bumps_slider * 512)) - ((int32_t)bumps_slider * 256);
            if (sawtooth < 0) sawtooth = -sawtooth;
            sawtooth -= ((int32_t)bumps_slider * 128);
            current_goal = sawtooth * spring_slider / 128;

            break;
        default:
            current_goal = 0;
            break;
    }
    set_duty_cycle(1, current_pid_tick(current_goal));
    D13 = 0;
}


int16_t main(void) {

    init_elecanisms();
    init_ajuart();


    // Pins for timing mgmt
    D12_DIR = OUT;
    D12 = 0;
    D13_DIR = OUT;
    D13 = 0;

    // Set direction pins
    D6_DIR = OUT;
    D6 = 1;
    D9_DIR = OUT;
    D9 = 1;

    // Enable the drivers
    D7_DIR = OUT;
    D7 = 0;
    D10_DIR = OUT;
    D10 = 0;

    // Set PWM pins (it'll get overwritten by PWM hardware)
    D5_DIR = OUT;
    D5 = 1;
    D8_DIR = OUT;
    D8 = 1;

    gain_p = 0;
    gain_i = 0.75 * 256;
    gain_d = 0;

    start_pwm();        // Start PWM on pin D5
    start_32b_timer();  // Start the 32-bit timer
    setup_encoder();    // Set up the rotary encoder
    current_pid_reset();

    USB_setup_vendor_callback = vendor_requests;
    init_usb();


    int32_t current;
    float current_amps;

    // About 3,072 Hz
    T1CON = 0x0000;         // 0000 0000 0001 0000 set Timer1 period
    PR1 = 0x1458;

    // About 1,024 Hz
    //T1CON = 0x0000;         // 0000 0000 0001 0000 set Timer1 period
    //PR1 = 0x3D08;

    // About 256 Hz
    //T1CON = 0x0010;         // 0000 0000 0001 0000 set Timer1 period
    //PR1 = 0x1E84;

    // About 32 Hz
    //T1CON = 0x0010;         // 0000 0000 0001 0000 set Timer1 period
    //PR1 = 0x7A11;

    TMR1 = 0;               // set Timer1 count to 0
    IFS0bits.T1IF = 0;      // lower Timer1 interrupt flag
    IEC0bits.T1IE = 1;      // enable Timer1 interrupt
    T1CONbits.TON = 1;      // turn on Timer1

    while(1) {
#ifndef USB_INTERRUPT
        usb_service();
#endif

        //printf("%ld\r\n", integral);
        //printf("%d\r\n", mode);
        printf("%ld\r\n", encoder_pos);
    }
}
