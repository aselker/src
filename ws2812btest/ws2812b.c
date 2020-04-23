#include "ws2812b.h"

#define WS2812B_OUT         D0
#define WS2812B_DIR         D0_DIR

void init_ws2812b(void) {
    WS2812B_DIR = OUT;
    WS2812B_OUT = 0;
}

void ws2812b_write(uint8_t red, uint8_t green, uint8_t blue) {
    // The complier loads red into register W0, green into register W1, 
    // and blue into register W2.

    // Write green, bit 7
    WS2812B_OUT = 1;
    __asm__("nop");         // 1
    __asm__("nop");         // 2
    __asm__("nop");         // 3
    __asm__("nop");         // 4
    __asm__("nop");         // 5
    __asm__("btss W1, #7"); // 6
    WS2812B_OUT = 0;        // 7 
    __asm__("nop");         // 8
    __asm__("nop");         // 9
    __asm__("nop");         // 10
    __asm__("nop");         // 11
    __asm__("nop");         // 12
    WS2812B_OUT = 0;        // 13
    __asm__("nop");         // 14
    __asm__("nop");         // 15
    __asm__("nop");         // 16
    __asm__("nop");         // 17
    __asm__("nop");         // 18

    // Write green, bit 6
    WS2812B_OUT = 1;
    __asm__("nop");         // 1
    __asm__("nop");         // 2
    __asm__("nop");         // 3
    __asm__("nop");         // 4
    __asm__("nop");         // 5
    __asm__("btss W1, #6"); // 6
    WS2812B_OUT = 0;        // 7 
    __asm__("nop");         // 8
    __asm__("nop");         // 9
    __asm__("nop");         // 10
    __asm__("nop");         // 11
    __asm__("nop");         // 12
    WS2812B_OUT = 0;        // 13
    __asm__("nop");         // 14
    __asm__("nop");         // 15
    __asm__("nop");         // 16
    __asm__("nop");         // 17
    __asm__("nop");         // 18

    // Write green, bit 5
    WS2812B_OUT = 1;
    __asm__("nop");         // 1
    __asm__("nop");         // 2
    __asm__("nop");         // 3
    __asm__("nop");         // 4
    __asm__("nop");         // 5
    __asm__("btss W1, #5"); // 6
    WS2812B_OUT = 0;        // 7 
    __asm__("nop");         // 8
    __asm__("nop");         // 9
    __asm__("nop");         // 10
    __asm__("nop");         // 11
    __asm__("nop");         // 12
    WS2812B_OUT = 0;        // 13
    __asm__("nop");         // 14
    __asm__("nop");         // 15
    __asm__("nop");         // 16
    __asm__("nop");         // 17
    __asm__("nop");         // 18

    // Write green, bit 4
    WS2812B_OUT = 1;
    __asm__("nop");         // 1
    __asm__("nop");         // 2
    __asm__("nop");         // 3
    __asm__("nop");         // 4
    __asm__("nop");         // 5
    __asm__("btss W1, #4"); // 6
    WS2812B_OUT = 0;        // 7 
    __asm__("nop");         // 8
    __asm__("nop");         // 9
    __asm__("nop");         // 10
    __asm__("nop");         // 11
    __asm__("nop");         // 12
    WS2812B_OUT = 0;        // 13
    __asm__("nop");         // 14
    __asm__("nop");         // 15
    __asm__("nop");         // 16
    __asm__("nop");         // 17
    __asm__("nop");         // 18

    // Write green, bit 3
    WS2812B_OUT = 1;
    __asm__("nop");         // 1
    __asm__("nop");         // 2
    __asm__("nop");         // 3
    __asm__("nop");         // 4
    __asm__("nop");         // 5
    __asm__("btss W1, #3"); // 6
    WS2812B_OUT = 0;        // 7 
    __asm__("nop");         // 8
    __asm__("nop");         // 9
    __asm__("nop");         // 10
    __asm__("nop");         // 11
    __asm__("nop");         // 12
    WS2812B_OUT = 0;        // 13
    __asm__("nop");         // 14
    __asm__("nop");         // 15
    __asm__("nop");         // 16
    __asm__("nop");         // 17
    __asm__("nop");         // 18

    // Write green, bit 2
    WS2812B_OUT = 1;
    __asm__("nop");         // 1
    __asm__("nop");         // 2
    __asm__("nop");         // 3
    __asm__("nop");         // 4
    __asm__("nop");         // 5
    __asm__("btss W1, #2"); // 6
    WS2812B_OUT = 0;        // 7 
    __asm__("nop");         // 8
    __asm__("nop");         // 9
    __asm__("nop");         // 10
    __asm__("nop");         // 11
    __asm__("nop");         // 12
    WS2812B_OUT = 0;        // 13
    __asm__("nop");         // 14
    __asm__("nop");         // 15
    __asm__("nop");         // 16
    __asm__("nop");         // 17
    __asm__("nop");         // 18

    // Write green, bit 1
    WS2812B_OUT = 1;
    __asm__("nop");         // 1
    __asm__("nop");         // 2
    __asm__("nop");         // 3
    __asm__("nop");         // 4
    __asm__("nop");         // 5
    __asm__("btss W1, #1"); // 6
    WS2812B_OUT = 0;        // 7 
    __asm__("nop");         // 8
    __asm__("nop");         // 9
    __asm__("nop");         // 10
    __asm__("nop");         // 11
    __asm__("nop");         // 12
    WS2812B_OUT = 0;        // 13
    __asm__("nop");         // 14
    __asm__("nop");         // 15
    __asm__("nop");         // 16
    __asm__("nop");         // 17
    __asm__("nop");         // 18

    // Write green, bit 0
    WS2812B_OUT = 1;
    __asm__("nop");         // 1
    __asm__("nop");         // 2
    __asm__("nop");         // 3
    __asm__("nop");         // 4
    __asm__("nop");         // 5
    __asm__("btss W1, #0"); // 6
    WS2812B_OUT = 0;        // 7 
    __asm__("nop");         // 8
    __asm__("nop");         // 9
    __asm__("nop");         // 10
    __asm__("nop");         // 11
    __asm__("nop");         // 12
    WS2812B_OUT = 0;        // 13
    __asm__("nop");         // 14
    __asm__("nop");         // 15
    __asm__("nop");         // 16
    __asm__("nop");         // 17
    __asm__("nop");         // 18

    // Write red, bit 7
    WS2812B_OUT = 1;
    __asm__("nop");         // 1
    __asm__("nop");         // 2
    __asm__("nop");         // 3
    __asm__("nop");         // 4
    __asm__("nop");         // 5
    __asm__("btss W0, #7"); // 6
    WS2812B_OUT = 0;        // 7 
    __asm__("nop");         // 8
    __asm__("nop");         // 9
    __asm__("nop");         // 10
    __asm__("nop");         // 11
    __asm__("nop");         // 12
    WS2812B_OUT = 0;        // 13
    __asm__("nop");         // 14
    __asm__("nop");         // 15
    __asm__("nop");         // 16
    __asm__("nop");         // 17
    __asm__("nop");         // 18

    // Write red, bit 6
    WS2812B_OUT = 1;
    __asm__("nop");         // 1
    __asm__("nop");         // 2
    __asm__("nop");         // 3
    __asm__("nop");         // 4
    __asm__("nop");         // 5
    __asm__("btss W0, #6"); // 6
    WS2812B_OUT = 0;        // 7 
    __asm__("nop");         // 8
    __asm__("nop");         // 9
    __asm__("nop");         // 10
    __asm__("nop");         // 11
    __asm__("nop");         // 12
    WS2812B_OUT = 0;        // 13
    __asm__("nop");         // 14
    __asm__("nop");         // 15
    __asm__("nop");         // 16
    __asm__("nop");         // 17
    __asm__("nop");         // 18

    // Write red, bit 5
    WS2812B_OUT = 1;
    __asm__("nop");         // 1
    __asm__("nop");         // 2
    __asm__("nop");         // 3
    __asm__("nop");         // 4
    __asm__("nop");         // 5
    __asm__("btss W0, #5"); // 6
    WS2812B_OUT = 0;        // 7 
    __asm__("nop");         // 8
    __asm__("nop");         // 9
    __asm__("nop");         // 10
    __asm__("nop");         // 11
    __asm__("nop");         // 12
    WS2812B_OUT = 0;        // 13
    __asm__("nop");         // 14
    __asm__("nop");         // 15
    __asm__("nop");         // 16
    __asm__("nop");         // 17
    __asm__("nop");         // 18

    // Write red, bit 4
    WS2812B_OUT = 1;
    __asm__("nop");         // 1
    __asm__("nop");         // 2
    __asm__("nop");         // 3
    __asm__("nop");         // 4
    __asm__("nop");         // 5
    __asm__("btss W0, #4"); // 6
    WS2812B_OUT = 0;        // 7 
    __asm__("nop");         // 8
    __asm__("nop");         // 9
    __asm__("nop");         // 10
    __asm__("nop");         // 11
    __asm__("nop");         // 12
    WS2812B_OUT = 0;        // 13
    __asm__("nop");         // 14
    __asm__("nop");         // 15
    __asm__("nop");         // 16
    __asm__("nop");         // 17
    __asm__("nop");         // 18

    // Write red, bit 3
    WS2812B_OUT = 1;
    __asm__("nop");         // 1
    __asm__("nop");         // 2
    __asm__("nop");         // 3
    __asm__("nop");         // 4
    __asm__("nop");         // 5
    __asm__("btss W0, #3"); // 6
    WS2812B_OUT = 0;        // 7 
    __asm__("nop");         // 8
    __asm__("nop");         // 9
    __asm__("nop");         // 10
    __asm__("nop");         // 11
    __asm__("nop");         // 12
    WS2812B_OUT = 0;        // 13
    __asm__("nop");         // 14
    __asm__("nop");         // 15
    __asm__("nop");         // 16
    __asm__("nop");         // 17
    __asm__("nop");         // 18

    // Write red, bit 2
    WS2812B_OUT = 1;
    __asm__("nop");         // 1
    __asm__("nop");         // 2
    __asm__("nop");         // 3
    __asm__("nop");         // 4
    __asm__("nop");         // 5
    __asm__("btss W0, #2"); // 6
    WS2812B_OUT = 0;        // 7 
    __asm__("nop");         // 8
    __asm__("nop");         // 9
    __asm__("nop");         // 10
    __asm__("nop");         // 11
    __asm__("nop");         // 12
    WS2812B_OUT = 0;        // 13
    __asm__("nop");         // 14
    __asm__("nop");         // 15
    __asm__("nop");         // 16
    __asm__("nop");         // 17
    __asm__("nop");         // 18

    // Write red, bit 1
    WS2812B_OUT = 1;
    __asm__("nop");         // 1
    __asm__("nop");         // 2
    __asm__("nop");         // 3
    __asm__("nop");         // 4
    __asm__("nop");         // 5
    __asm__("btss W0, #1"); // 6
    WS2812B_OUT = 0;        // 7 
    __asm__("nop");         // 8
    __asm__("nop");         // 9
    __asm__("nop");         // 10
    __asm__("nop");         // 11
    __asm__("nop");         // 12
    WS2812B_OUT = 0;        // 13
    __asm__("nop");         // 14
    __asm__("nop");         // 15
    __asm__("nop");         // 16
    __asm__("nop");         // 17
    __asm__("nop");         // 18

    // Write red, bit 0
    WS2812B_OUT = 1;
    __asm__("nop");         // 1
    __asm__("nop");         // 2
    __asm__("nop");         // 3
    __asm__("nop");         // 4
    __asm__("nop");         // 5
    __asm__("btss W0, #0"); // 6
    WS2812B_OUT = 0;        // 7 
    __asm__("nop");         // 8
    __asm__("nop");         // 9
    __asm__("nop");         // 10
    __asm__("nop");         // 11
    __asm__("nop");         // 12
    WS2812B_OUT = 0;        // 13
    __asm__("nop");         // 14
    __asm__("nop");         // 15
    __asm__("nop");         // 16
    __asm__("nop");         // 17
    __asm__("nop");         // 18

    // Write blue, bit 7
    WS2812B_OUT = 1;
    __asm__("nop");         // 1
    __asm__("nop");         // 2
    __asm__("nop");         // 3
    __asm__("nop");         // 4
    __asm__("nop");         // 5
    __asm__("btss W2, #7"); // 6
    WS2812B_OUT = 0;        // 7 
    __asm__("nop");         // 8
    __asm__("nop");         // 9
    __asm__("nop");         // 10
    __asm__("nop");         // 11
    __asm__("nop");         // 12
    WS2812B_OUT = 0;        // 13
    __asm__("nop");         // 14
    __asm__("nop");         // 15
    __asm__("nop");         // 16
    __asm__("nop");         // 17
    __asm__("nop");         // 18

    // Write blue, bit 6
    WS2812B_OUT = 1;
    __asm__("nop");         // 1
    __asm__("nop");         // 2
    __asm__("nop");         // 3
    __asm__("nop");         // 4
    __asm__("nop");         // 5
    __asm__("btss W2, #6"); // 6
    WS2812B_OUT = 0;        // 7 
    __asm__("nop");         // 8
    __asm__("nop");         // 9
    __asm__("nop");         // 10
    __asm__("nop");         // 11
    __asm__("nop");         // 12
    WS2812B_OUT = 0;        // 13
    __asm__("nop");         // 14
    __asm__("nop");         // 15
    __asm__("nop");         // 16
    __asm__("nop");         // 17
    __asm__("nop");         // 18

    // Write blue, bit 5
    WS2812B_OUT = 1;
    __asm__("nop");         // 1
    __asm__("nop");         // 2
    __asm__("nop");         // 3
    __asm__("nop");         // 4
    __asm__("nop");         // 5
    __asm__("btss W2, #5"); // 6
    WS2812B_OUT = 0;        // 7 
    __asm__("nop");         // 8
    __asm__("nop");         // 9
    __asm__("nop");         // 10
    __asm__("nop");         // 11
    __asm__("nop");         // 12
    WS2812B_OUT = 0;        // 13
    __asm__("nop");         // 14
    __asm__("nop");         // 15
    __asm__("nop");         // 16
    __asm__("nop");         // 17
    __asm__("nop");         // 18

    // Write blue, bit 4
    WS2812B_OUT = 1;
    __asm__("nop");         // 1
    __asm__("nop");         // 2
    __asm__("nop");         // 3
    __asm__("nop");         // 4
    __asm__("nop");         // 5
    __asm__("btss W2, #4"); // 6
    WS2812B_OUT = 0;        // 7 
    __asm__("nop");         // 8
    __asm__("nop");         // 9
    __asm__("nop");         // 10
    __asm__("nop");         // 11
    __asm__("nop");         // 12
    WS2812B_OUT = 0;        // 13
    __asm__("nop");         // 14
    __asm__("nop");         // 15
    __asm__("nop");         // 16
    __asm__("nop");         // 17
    __asm__("nop");         // 18

    // Write blue, bit 3
    WS2812B_OUT = 1;
    __asm__("nop");         // 1
    __asm__("nop");         // 2
    __asm__("nop");         // 3
    __asm__("nop");         // 4
    __asm__("nop");         // 5
    __asm__("btss W2, #3"); // 6
    WS2812B_OUT = 0;        // 7 
    __asm__("nop");         // 8
    __asm__("nop");         // 9
    __asm__("nop");         // 10
    __asm__("nop");         // 11
    __asm__("nop");         // 12
    WS2812B_OUT = 0;        // 13
    __asm__("nop");         // 14
    __asm__("nop");         // 15
    __asm__("nop");         // 16
    __asm__("nop");         // 17
    __asm__("nop");         // 18

    // Write blue, bit 2
    WS2812B_OUT = 1;
    __asm__("nop");         // 1
    __asm__("nop");         // 2
    __asm__("nop");         // 3
    __asm__("nop");         // 4
    __asm__("nop");         // 5
    __asm__("btss W2, #2"); // 6
    WS2812B_OUT = 0;        // 7 
    __asm__("nop");         // 8
    __asm__("nop");         // 9
    __asm__("nop");         // 10
    __asm__("nop");         // 11
    __asm__("nop");         // 12
    WS2812B_OUT = 0;        // 13
    __asm__("nop");         // 14
    __asm__("nop");         // 15
    __asm__("nop");         // 16
    __asm__("nop");         // 17
    __asm__("nop");         // 18

    // Write blue, bit 1
    WS2812B_OUT = 1;
    __asm__("nop");         // 1
    __asm__("nop");         // 2
    __asm__("nop");         // 3
    __asm__("nop");         // 4
    __asm__("nop");         // 5
    __asm__("btss W2, #1"); // 6
    WS2812B_OUT = 0;        // 7 
    __asm__("nop");         // 8
    __asm__("nop");         // 9
    __asm__("nop");         // 10
    __asm__("nop");         // 11
    __asm__("nop");         // 12
    WS2812B_OUT = 0;        // 13
    __asm__("nop");         // 14
    __asm__("nop");         // 15
    __asm__("nop");         // 16
    __asm__("nop");         // 17
    __asm__("nop");         // 18

    // Write blue, bit 0
    WS2812B_OUT = 1;
    __asm__("nop");         // 1
    __asm__("nop");         // 2
    __asm__("nop");         // 3
    __asm__("nop");         // 4
    __asm__("nop");         // 5
    __asm__("btss W2, #0"); // 6
    WS2812B_OUT = 0;        // 7 
    __asm__("nop");         // 8
    __asm__("nop");         // 9
    __asm__("nop");         // 10
    __asm__("nop");         // 11
    __asm__("nop");         // 12
    WS2812B_OUT = 0;        // 13
    __asm__("nop");         // 14
    __asm__("nop");         // 15
    __asm__("nop");         // 16
    __asm__("nop");         // 17
    __asm__("nop");         // 18
}

void ws2812b_clear(uint16_t num) {
    uint16_t i;

    disable_interrupts();
    for (i = 0; i < num; i++)
        ws2812b_write(0, 0, 0);
    enable_interrupts();
}

