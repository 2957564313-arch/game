#ifndef __GAME_H
#define __GAME_H

#include "stm32f10x.h"

// 游戏对象结构体
typedef struct {
    uint8_t x;          // x坐标 (0-15)
    uint8_t y;          // y坐标 (0-2)
    uint8_t width;      // 宽度
    uint8_t height;     // 高度
    uint8_t lives;      // 生命数
    uint8_t speed;      // 移动速度
} Player_t;

typedef struct {
    uint8_t active;     // 是否活跃
    uint8_t x;          // x坐标 (0-15)
    uint8_t y;          // y坐标 (0-2)
    uint8_t width;      // 宽度
    uint8_t height;     // 高度
    uint8_t speed;      // 移动速度
    uint8_t score;      // 得分
} Enemy_t;

typedef struct {
    uint8_t active;     // 是否活跃
    uint8_t x;          // x坐标 (0-15)
    uint8_t y;          // y坐标 (0-2)
    uint8_t speed;      // 移动速度
} Bullet_t;

typedef struct {
    uint16_t score;                 // 分数
    uint8_t game_over;              // 游戏结束标志
    uint16_t enemy_spawn_timer;     // 敌人生成计时器
    uint16_t enemy_spawn_interval;  // 敌人生成间隔
    uint8_t explosion_frame;        // 爆炸动画帧
    uint8_t explosion_x;            // 爆炸x坐标
    uint8_t explosion_y;            // 爆炸y坐标
    uint8_t show_explosion;         // 显示爆炸标志
} Game_t;

// 游戏常量
#define MAX_ENEMIES 5   // 最大敌人数量
#define MAX_BULLETS 3   // 最大子弹数量

// 外部变量声明
extern Player_t player;
extern Enemy_t enemies[MAX_ENEMIES];
extern Bullet_t bullets[MAX_BULLETS];
extern Game_t game;

// 函数声明
void Game_Init(void);
void Game_Loop(void);

#endif
