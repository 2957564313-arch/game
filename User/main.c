#include "stm32f10x.h"
#include "Key.h"
#include "display.h"
#include "game.h"
#include "Delay.h"

int main(void)
{
    // 初始化外设
    Key_Init();
    Display_Init();
    
    // 显示游戏开始画面
    Draw_GameStart();
    
    // 等待按键开始游戏
    while(Key_Scan() == 0);
    Delay_ms(200); // 消抖
    
    // 初始化游戏
    Game_Init();
    
    // 开始游戏主循环
    Game_Loop();
    
    while(1)
    {
        // 游戏结束后等待按键重启
        if(Key_Scan() != 0)
        {
            Delay_ms(200); // 消抖
            
            // 重新初始化游戏
            Game_Init();
            
            // 重新开始游戏主循环
            Game_Loop();
        }
    }
}
