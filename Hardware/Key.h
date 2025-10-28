#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"

// 按键定义
#define KEY_LEFT    GPIO_Pin_0
#define KEY_RIGHT   GPIO_Pin_1
#define KEY_FIRE    GPIO_Pin_2

// 函数声明
void KEY_Init(void);
uint8_t KEY_ReadLeft(void);
uint8_t KEY_ReadRight(void);
uint8_t KEY_ReadFire(void);

#endif
