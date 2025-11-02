#include "stm32f10x.h"
#include "OLED.h"
#include "game.h"
#include <string.h>
#include "Delay.h"

// 帧缓冲区 - 存储当前屏幕内容
static char screen_buffer[4][17]; // 4行，每行16字符+1结束符

// 初始化显示
void Display_Init(void)
{
    OLED_Init();
    OLED_Clear();
    
    // 初始化屏幕缓冲区
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 16; j++) {
            screen_buffer[i][j] = ' ';
        }
        screen_buffer[i][16] = '\0';
    }
}

// 清除整个屏幕
void Clear_Screen(void)
{
    OLED_Clear();
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 16; j++) {
            screen_buffer[i][j] = ' ';
        }
        screen_buffer[i][16] = '\0';
    }
}

// 更新屏幕显示
void Update_Screen(void)
{
    for(int i = 0; i < 4; i++) {
        OLED_ShowString(i + 1, 1, screen_buffer[i]);
    }
}

// 在指定位置设置字符
void Set_Char(uint8_t row, uint8_t col, char ch)
{
    if(row < 4 && col < 16) {
        screen_buffer[row][col] = ch;
    }
}

// 清除指定区域
void Clear_Area(uint8_t start_row, uint8_t start_col, uint8_t end_row, uint8_t end_col)
{
    for(uint8_t row = start_row; row <= end_row && row < 4; row++) {
        for(uint8_t col = start_col; col <= end_col && col < 16; col++) {
            screen_buffer[row][col] = ' ';
        }
    }
}

// 绘制玩家飞机
void Draw_Player(Player_t *player)
{
    // 清除玩家区域 (1x1)
    Clear_Area(player->y, player->x, player->y, player->x);
    
    // 绘制飞机 (使用简单字符表示)
    Set_Char(player->y, player->x, '>');
}

// 绘制敌人
void Draw_Enemy(Enemy_t *enemy)
{
    if(enemy->active) {
        Set_Char(enemy->y, enemy->x, '*');
    }
}

// 清除敌人
void Clear_Enemy(Enemy_t *enemy)
{
    Set_Char(enemy->y, enemy->x, ' ');
}

// 绘制子弹
void Draw_Bullet(Bullet_t *bullet)
{
    if(bullet->active) {
        Set_Char(bullet->y, bullet->x, '-');
    }
}

// 清除子弹
void Clear_Bullet(Bullet_t *bullet)
{
    Set_Char(bullet->y, bullet->x, ' ');
}

// 绘制爆炸效果
void Draw_Explosion(uint8_t x, uint8_t y, uint8_t frame)
{
    // 简单的爆炸效果
    switch(frame) {
        case 0:
            Set_Char(y, x, '*');
            break;
        case 1:
            if(x > 0) Set_Char(y, x - 1, '*');
            if(x < 15) Set_Char(y, x + 1, '*');
            break;
        case 2:
            if(x > 0) Set_Char(y, x - 1, ' ');
            if(x < 15) Set_Char(y, x + 1, ' ');
            Set_Char(y, x, ' ');
            break;
    }
}

// 更新分数显示
void Update_Score(uint16_t score)
{
    // 格式化分数显示
    char score_str[17] = "SCORE:";
    
    // 将分数转换为字符串
    uint16_t temp_score = score;
    uint8_t digits = 0;
    char num_str[6];
    
    if(temp_score == 0) {
        num_str[0] = '0';
        digits = 1;
    } else {
        while(temp_score > 0 && digits < 5) {
            num_str[digits] = '0' + (temp_score % 10);
            temp_score /= 10;
            digits++;
        }
    }
    
    // 填充空格
    uint8_t i;
    for(i = 0; i < 10 - digits; i++) {
        score_str[6 + i] = ' ';
    }
    
    // 反转数字字符串
    for(i = 0; i < digits; i++) {
        score_str[6 + (10 - digits) + i] = num_str[digits - 1 - i];
    }
    
    score_str[16] = '\0';
    
    // 更新屏幕缓冲区
    for(i = 0; i < 16; i++) {
        screen_buffer[3][i] = score_str[i];
    }
}

// 绘制游戏结束画面
// 绘制游戏结束画面
// 绘制游戏结束画面
void Draw_GameOver(uint16_t score)
{
    Clear_Screen();
    
    // 显示游戏结束标题
    OLED_ShowString(1, 1, "   GAME OVER   ");
    
    // 显示分数
    char score_line[17] = "SCORE:         ";
    
    // 将分数转换为字符串
    uint16_t temp_score = score;
    uint8_t digits = 0;
    char num_str[6];
    
    if(temp_score == 0) {
        num_str[0] = '0';
        digits = 1;
    } else {
        while(temp_score > 0 && digits < 5) {
            num_str[digits] = '0' + (temp_score % 10);
            temp_score /= 10;
            digits++;
        }
    }
    
    // 将数字填入字符串
    for(uint8_t i = 0; i < digits; i++) {
        score_line[6 + i] = num_str[digits - 1 - i];
    }
    
    OLED_ShowString(2, 1, score_line);
	Delay_s(3);
	
}

// 绘制游戏开始画面
void Draw_GameStart(void)
{
    Clear_Screen();
    
    OLED_ShowString(1, 1, "  AIRPLANE WAR ");
    OLED_ShowString(2, 1, "               ");
    OLED_ShowString(3, 1, "  PRESS ANY    ");
    OLED_ShowString(4, 1, "    KEY START  ");
}
