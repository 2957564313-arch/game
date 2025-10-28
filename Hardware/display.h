#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "stm32f10x.h"

// 函数声明
void DISPLAY_Init(void);
void DISPLAY_ShowMenu(uint16_t highScore);
void DISPLAY_ShowGame(uint8_t gameScreen[3][16], uint16_t score);
void DISPLAY_ShowGameOver(uint16_t score);

#endif
