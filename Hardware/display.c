#include "stm32f10x.h"                  // Device header
#include "display.h"
#include "oled.h"

void DISPLAY_Init(void) {
    OLED_Init();
    OLED_Clear();
}

void DISPLAY_ShowMenu(uint16_t highScore) {
    OLED_Clear();
    
    OLED_ShowString(1, 1, "PLANE GAME");
    OLED_ShowString(1, 2, "PRESS FIRE");
    OLED_ShowString(1, 3, "HIGH:");
    OLED_ShowNum(7, 3, highScore, 5);
    
    // 第4行显示提示
    OLED_ShowString(1, 4, "A-MOVE | FIRE");
}

void DISPLAY_ShowGame(uint8_t gameScreen[3][16], uint16_t score) {
    OLED_Clear();
    
    // 显示游戏区域（前3行）
    for(uint8_t y = 0; y < 3; y++) {
        char line[17] = {0};
        for(uint8_t x = 0; x < 16; x++) {
            line[x] = gameScreen[y][x];
        }
        OLED_ShowString(1, y + 1, line);
    }
    
    // 第4行显示分数
    OLED_ShowString(1, 4, "SCORE:");
    OLED_ShowNum(7, 4, score, 5);
}

void DISPLAY_ShowGameOver(uint16_t score) {
    OLED_Clear();
    
    OLED_ShowString(1, 1, "GAME OVER");
    OLED_ShowString(1, 2, "SCORE:");
    OLED_ShowNum(7, 2, score, 5);
    OLED_ShowString(1, 3, "PRESS FIRE");
    OLED_ShowString(1, 4, "TO RESTART");
}
