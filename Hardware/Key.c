#include "stm32f10x.h"
#include "Delay.h"
#include "Key.h"

void Key_Init(void)
{
    // 使能GPIOA和GPIOC时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // PA0 上移按键
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // PA2 下移按键
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // PC14 发射按键
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

// 改进的按键扫描函数
// 返回值：0-无按键按下，1-上移，2-下移，3-发射
uint8_t Key_Scan(void)
{
    static uint8_t key_up_last = 1;
    static uint8_t key_down_last = 1;
    static uint8_t key_fire_last = 1;
    
    uint8_t key_up_current = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2);
    uint8_t key_down_current = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
    uint8_t key_fire_current = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14);
    
    // 检测按键按下（下降沿）
    if(key_up_current == 0 && key_up_last == 1) {
        key_up_last = 0;
        Delay_ms(10); // 消抖
        return 1;
    }
    key_up_last = key_up_current;
    
    if(key_down_current == 0 && key_down_last == 1) {
        key_down_last = 0;
        Delay_ms(10);
        return 2;
    }
    key_down_last = key_down_current;
    
    if(key_fire_current == 0 && key_fire_last == 1) {
        key_fire_last = 0;
        Delay_ms(10);
        return 3;
    }
    key_fire_last = key_fire_current;
    
    return 0;
}
