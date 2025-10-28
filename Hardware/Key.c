#include "stm32f10x.h"                  // Device header
#include "key.h"

void KEY_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 开启GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // 配置按键引脚为上拉输入
    GPIO_InitStructure.GPIO_Pin = KEY_LEFT | KEY_RIGHT | KEY_FIRE;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

uint8_t KEY_ReadLeft(void) {
    return (GPIO_ReadInputDataBit(GPIOA, KEY_LEFT) == Bit_RESET);
}

uint8_t KEY_ReadRight(void) {
    return (GPIO_ReadInputDataBit(GPIOA, KEY_RIGHT) == Bit_RESET);
}

uint8_t KEY_ReadFire(void) {
    return (GPIO_ReadInputDataBit(GPIOA, KEY_FIRE) == Bit_RESET);
}
	