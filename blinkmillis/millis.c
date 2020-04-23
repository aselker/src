#include "millis.h"

uint32_t milliseconds;

void __attribute__((interrupt, auto_psv)) _T1Interrupt(void) {
    IFS0bits.T1IF = 0;      // lower Timer1 interrupt flag

    milliseconds++;
}

void init_millis(void) {
    milliseconds = 0;

    T1CON = 0x0000;         // set Timer1 period to be 1ms
    PR1 = 0x3E7F;
    
    TMR1 = 0;               // set Timer1 count to 0
    IFS0bits.T1IF = 0;      // lower Timer1 interrupt flag
    IEC0bits.T1IE = 1;      // enable Timer1 interrupt
    T1CONbits.TON = 1;      // turn on Timer1;
}

uint32_t millis(void) {
    return milliseconds;
}

