#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "stm32f10x.h"
#include "game.h"  // 包含game.h以使用类型定义

// 初始化显示
void Display_Init(void);

// 清除整个屏幕
void Clear_Screen(void);

// 更新屏幕显示
void Update_Screen(void);

// 在指定位置设置字符
void Set_Char(uint8_t row, uint8_t col, char ch);

// 清除指定区域
void Clear_Area(uint8_t start_row, uint8_t start_col, uint8_t end_row, uint8_t end_col);

// 绘制玩家飞机
void Draw_Player(Player_t *player);

// 绘制敌人
void Draw_Enemy(Enemy_t *enemy);

// 清除敌人
void Clear_Enemy(Enemy_t *enemy);

// 绘制子弹
void Draw_Bullet(Bullet_t *bullet);

// 清除子弹
void Clear_Bullet(Bullet_t *bullet);

// 绘制爆炸效果
void Draw_Explosion(uint8_t x, uint8_t y, uint8_t frame);

// 更新分数显示
void Update_Score(uint16_t score);

// 绘制游戏结束画面
void Draw_GameOver(uint16_t score);

// 绘制游戏开始画面
void Draw_GameStart(void);

#endif
