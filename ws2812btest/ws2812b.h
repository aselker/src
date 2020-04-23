
#ifndef _WS2812B_H_
#define _WS2812B_H_

#include "elecanisms.h"

#define WS2812B_OUT         D0
#define WS2812B_OUT_DIR     D0_OUT

void init_ws2812b(void);
void ws2812b_write(uint8_t red, uint8_t green, uint8_t blue);
void ws2812b_clear(uint16_t num);

#endif

