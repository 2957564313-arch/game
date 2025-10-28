#include "stm32f10x.h"
#include "game.h"
#include "key.h"
#include "display.h"
#include "Delay.h"

volatile uint32_t msTicks = 0;

int main(void) {
    // 系统初始化
    SystemInit();
    
    // 配置SysTick为1ms中断
    if(SysTick_Config(SystemCoreClock / 1000)) {
        while(1);
    }
    
    // 模块初始化
    KEY_Init();
    DISPLAY_Init();
    GAME_Init();
    
    // 主循环
    while(1) {
        GAME_Update();
        Delay_ms(100); // 降低更新频率以适应字符显示
    }
}
