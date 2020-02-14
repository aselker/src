#include "elecanisms.h"
#include "ajuart.h"
#include <stdio.h>

#define RES_VAL 0.075
#define VREF 3.3

int16_t main(void) {
    init_elecanisms();
    init_ajuart();

    T1CON = 0x0030;         // set Timer1 period to 0.5s
    PR1 = 0x7A11;

    TMR1 = 0;               // set Timer1 count to 0
    IFS0bits.T1IF = 0;      // lower Timer1 interrupt flag
    T1CONbits.TON = 1;      // turn on Timer1

    LED2 = ON;

    float scaling = VREF/1024;

    while (1) {
        // if (IFS0bits.T1IF == 1 && D2 == 0) {
        //     IFS0bits.T1IF = 0;      // lower Timer1 interrupt flag
        //     LED2 = !LED2;           // toggle LED2
        // }
        /*LED1 = (SW2 == 0) ? ON : OFF;   // turn LED1 on if SW2 is pressed */
        /*LED3 = (SW3 == 0) ? ON : OFF;   // turn LED3 on if SW3 is pressed*/
        float measuredvoltage = (float)read_analog(A0_AN)*scaling;
        float vdrop = (measuredvoltage - VREF/2)/10;
        float current = vdrop/RES_VAL;
        printf("A0:%u\tV:%f\tDrop:%f\tCurrent:%f\r\n",read_analog(A0_AN),measuredvoltage,vdrop,current);
        //printf("hello\r\n");
    }
}
