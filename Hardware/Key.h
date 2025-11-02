#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"

// 按键初始化函数
void Key_Init(void);

// 按键扫描函数
// 返回值：0-无按键按下，1-上移(PA0)，2-下移(PA2)，3-发射(PC14)
uint8_t Key_Scan(void);

#endif
