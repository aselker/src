#include "elecanisms.h"
#include "ajuart.h"
#include <stdio.h>

int16_t main(void) {
    uint16_t sw1_pressed, sw2_pressed, sw3_pressed;
    uint8_t ch;    

    init_elecanisms();
    init_ajuart();

    sw1_pressed = FALSE;
    sw2_pressed = FALSE;
    sw3_pressed = FALSE;

    // Configure Timer1 period to be 20 ms
    T1CON = 0x0010;
    PR1 = 0x9C3F;

    // Start Timer1
    TMR1 = 0;
    IFS0bits.T1IF = 0;
    T1CONbits.TON = 1;

    while (1) {
        // Sample switches when Timer1 goes off to implement debouncing
        if (IFS0bits.T1IF == 1) {
            IFS0bits.T1IF = 0;      // Lower Timer1 interrupt flag

            if ((SW1 == 0) && !sw1_pressed) {
                sw1_pressed = TRUE;
                printf("SW1 down!!\r\n");
            } else if ((SW1 == 1) && sw1_pressed) {
                sw1_pressed = FALSE;
                printf("SW1 up!!\r\n");
            }

            if ((SW2 == 0) && !sw2_pressed) {
                sw2_pressed = TRUE;
                printf("SW2 down!!\r\n");
            } else if ((SW2 == 1) && sw2_pressed) {
                sw2_pressed = FALSE;
                printf("SW2 up!!\r\n");
            }

            if ((SW3 == 0) && !sw3_pressed) {
                sw3_pressed = TRUE;
                printf("SW3 down!!\r\n");
            } else if ((SW3 == 1) && sw3_pressed) {
                sw3_pressed = FALSE;
                printf("SW3 up!!\r\n");
            }
        }

        if (U1_in_waiting() > 0) {
            ch = U1_getc();
            if (ch == '1')
                LED1 = !LED1;
            else if (ch == '2')
                LED2 = !LED2;
            else if (ch == '3')
                LED3 = !LED3;
        }
    }
}
