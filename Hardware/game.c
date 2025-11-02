#include "stm32f10x.h"
#include "game.h"
#include "Key.h"
#include "display.h"
#include "Delay.h"
#include <stdlib.h>

// 游戏对象定义
Player_t player;
Enemy_t enemies[MAX_ENEMIES];
Bullet_t bullets[MAX_BULLETS];
Game_t game;

// 初始化玩家
void Init_Player(void)
{
    player.x = 1;          // x坐标 (列)
    player.y = 1;          // y坐标 (行)
    player.width = 1;      // 宽度
    player.height = 1;     // 高度
    player.lives = 3;      // 生命数
    player.speed = 1;      // 移动速度
}

// 初始化敌人
void Init_Enemies(void)
{
    uint8_t i;
    for(i = 0; i < MAX_ENEMIES; i++)
    {
        enemies[i].active = 0;
    }
}

// 初始化子弹
void Init_Bullets(void)
{
    uint8_t i;
    for(i = 0; i < MAX_BULLETS; i++)
    {
        bullets[i].active = 0;
        bullets[i].speed = 2;
    }
}

// 初始化游戏
void Game_Init(void)
{
    Init_Player();
    Init_Enemies();
    Init_Bullets();
    
    game.score = 0;
    game.game_over = 0;
    game.enemy_spawn_timer = 0;
    game.enemy_spawn_interval = 30; // 敌人生成间隔
    game.explosion_frame = 0;
    game.explosion_x = 0;
    game.explosion_y = 0;
    game.show_explosion = 0;
    
    // 初始化屏幕
    Clear_Screen();
    
    // 立即绘制玩家和分数
    Draw_Player(&player);
    Update_Score(game.score);
    Update_Screen();
}

// 生成敌人
void Spawn_Enemy(void)
{
    uint8_t i;
    for(i = 0; i < MAX_ENEMIES; i++)
    {
        if(!enemies[i].active)
        {
            enemies[i].active = 1;
            enemies[i].x = 15; // 从屏幕右侧出现
            enemies[i].y = rand() % 3; // 随机y坐标（0-2行）
            enemies[i].speed = 1;
            enemies[i].score = 10;
            break;
        }
    }
}

// 移动敌人
void Move_Enemies(void)
{
    uint8_t i;
    for(i = 0; i < MAX_ENEMIES; i++)
    {
        if(enemies[i].active)
        {
            // 清除敌人当前位置
            Clear_Enemy(&enemies[i]);
            
            // 移动敌人
            enemies[i].x -= enemies[i].speed;
            
            // 检查敌人是否移出屏幕
            if(enemies[i].x >= 16)
            {
                enemies[i].active = 0;
            }
            else
            {
                // 绘制敌人新位置
                Draw_Enemy(&enemies[i]);
            }
        }
    }
}

// 发射子弹
void Fire_Bullet(void)
{
    uint8_t i;
    for(i = 0; i < MAX_BULLETS; i++)
    {
        if(!bullets[i].active)
        {
            bullets[i].active = 1;
            bullets[i].x = player.x + 1; // 从飞机右侧发射
            bullets[i].y = player.y;     // 与飞机同一行
            break;
        }
    }
}

// 移动子弹
void Move_Bullets(void)
{
    uint8_t i;
    for(i = 0; i < MAX_BULLETS; i++)
    {
        if(bullets[i].active)
        {
            // 清除子弹当前位置
            Clear_Bullet(&bullets[i]);
            
            // 移动子弹
            bullets[i].x += bullets[i].speed;
            
            // 检查子弹是否移出屏幕
            if(bullets[i].x >= 16)
            {
                bullets[i].active = 0;
            }
            else
            {
                // 绘制子弹新位置
                Draw_Bullet(&bullets[i]);
            }
        }
    }
}

// 改进的碰撞检测 - 检测子弹与敌人碰撞
void Check_Bullet_Enemy_Collision(void)
{
    uint8_t i, j;
    for(i = 0; i < MAX_BULLETS; i++)
    {
        if(bullets[i].active)
        {
            for(j = 0; j < MAX_ENEMIES; j++)
            {
                if(enemies[j].active)
                {
                    // 改进碰撞检测 - 更宽松的条件
                    if(bullets[i].x >= enemies[j].x && 
                       bullets[i].x <= enemies[j].x + 1 &&
                       bullets[i].y == enemies[j].y)
                    {
                        // 碰撞发生
                        bullets[i].active = 0;
                        Clear_Bullet(&bullets[i]);
                        
                        enemies[j].active = 0;
                        Clear_Enemy(&enemies[j]);
                        
                        // 显示爆炸效果
                        game.show_explosion = 1;
                        game.explosion_x = enemies[j].x;
                        game.explosion_y = enemies[j].y;
                        game.explosion_frame = 0;
                        
                        // 增加分数
                        game.score += enemies[j].score;
                        
                        break;
                    }
                }
            }
        }
    }
}

// 检测玩家与敌人碰撞
void Check_Player_Enemy_Collision(void)
{
    uint8_t i;
    for(i = 0; i < MAX_ENEMIES; i++)
    {
        if(enemies[i].active)
        {
            // 改进碰撞检测
            if((enemies[i].x == player.x || enemies[i].x == player.x + 1) && 
               enemies[i].y == player.y)
            {
                // 碰撞发生
                enemies[i].active = 0;
                Clear_Enemy(&enemies[i]);
                
                // 显示爆炸效果
                game.show_explosion = 1;
                game.explosion_x = player.x;
                game.explosion_y = player.y;
                game.explosion_frame = 0;
                
                // 减少生命
                player.lives--;
                
                if(player.lives <= 0)
                {
                    game.game_over = 1;
                }
                else
                {
                    // 重置玩家位置
                    player.x = 1;
                    player.y = 1;
                    Draw_Player(&player);
                }
                
                break;
            }
        }
    }
}

// 更新爆炸效果
void Update_Explosion(void)
{
    if(game.show_explosion)
    {
        Draw_Explosion(game.explosion_x, game.explosion_y, game.explosion_frame);
        game.explosion_frame++;
        
        if(game.explosion_frame >= 3)
        {
            game.show_explosion = 0;
            Clear_Area(game.explosion_y, game.explosion_x, game.explosion_y, game.explosion_x);
        }
    }
}

// 处理玩家输入
void Handle_Input(void)
{
    uint8_t key = Key_Scan();
    
    switch(key)
    {
        case 1: // 上移
            if(player.y > 0)
            {
                Clear_Area(player.y, player.x, player.y, player.x);
                player.y--;
                Draw_Player(&player);
            }
            break;
        case 2: // 下移
            if(player.y < 2)
            {
                Clear_Area(player.y, player.x, player.y, player.x);
                player.y++;
                Draw_Player(&player);
            }
            break;
        case 3: // 发射
            Fire_Bullet();
            break;
    }
}

// 游戏主循环
void Game_Loop(void)
{
    uint32_t loop_counter = 0;
    
    while(!game.game_over)
    {
        // 处理输入
        Handle_Input();
        
        // 生成敌人
        game.enemy_spawn_timer++;
        if(game.enemy_spawn_timer >= game.enemy_spawn_interval)
        {
            Spawn_Enemy();
            game.enemy_spawn_timer = 0;
        }
        
        // 移动敌人
        if(loop_counter % 2 == 0) { // 每2帧移动一次敌人
            Move_Enemies();
        }
        
        // 移动子弹
        Move_Bullets();
        
        // 检测碰撞
        Check_Bullet_Enemy_Collision();
        Check_Player_Enemy_Collision();
        
        // 更新爆炸效果
        Update_Explosion();
        
        // 更新分数显示
        Update_Score(game.score);
        
        // 更新屏幕显示
        Update_Screen();
        
        // 控制游戏速度
        Delay_ms(50); // 加快游戏速度
        
        loop_counter++;
    }
    
    // 游戏结束 - 确保分数正确显示
    Draw_GameOver(game.score);
    Update_Screen(); // 确保更新到OLED
}
