#ifndef __GAME_H
#define __GAME_H

#include "stm32f10x.h"

// 游戏常量
#define GAME_WIDTH 16
#define GAME_HEIGHT 3  // 第4行用于显示分数

// 游戏对象
typedef struct {
    uint8_t x;
    uint8_t y;
} Player;

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t active;
} Bullet;

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t active;
} Enemy;

// 游戏状态
typedef enum {
    GAME_MENU,
    GAME_PLAYING,
    GAME_OVER
} Game_State;

// 函数声明
void GAME_Init(void);
void GAME_Update(void);
void GAME_Reset(void);

#endif
