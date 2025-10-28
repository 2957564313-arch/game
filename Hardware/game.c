#include "stm32f10x.h"                  // Device header
#include "game.h"
#include "key.h"
#include "display.h"

// 游戏变量
static Player player;
static Bullet bullets[2];  // 减少子弹数量
static Enemy enemies[3];   // 减少敌机数量
static uint16_t score = 0;
static Game_State gameState = GAME_MENU;
static uint32_t lastEnemySpawn = 0;
static uint8_t gameScreen[GAME_HEIGHT][GAME_WIDTH];

extern volatile uint32_t msTicks;

void GAME_Init(void) {
    GAME_Reset();
}

void GAME_Reset(void) {
    // 初始化玩家
    player.x = GAME_WIDTH / 2;
    player.y = GAME_HEIGHT - 1;
    
    // 初始化子弹
    for(uint8_t i = 0; i < 2; i++) {
        bullets[i].active = 0;
    }
    
    // 初始化敌机
    for(uint8_t i = 0; i < 3; i++) {
        enemies[i].active = 0;
    }
    
    // 清空游戏屏幕
    for(uint8_t y = 0; y < GAME_HEIGHT; y++) {
        for(uint8_t x = 0; x < GAME_WIDTH; x++) {
            gameScreen[y][x] = ' ';
        }
    }
    
    score = 0;
    gameState = GAME_MENU;
    lastEnemySpawn = msTicks;
}

static void ClearGameScreen(void) {
    for(uint8_t y = 0; y < GAME_HEIGHT; y++) {
        for(uint8_t x = 0; x < GAME_WIDTH; x++) {
            gameScreen[y][x] = ' ';
        }
    }
}

static void UpdatePlayer(void) {
    static uint32_t lastMoveTime = 0;
    
    if((msTicks - lastMoveTime) > 200) { // 控制移动速度
        if(KEY_ReadLeft() && player.x > 0) {
            player.x--;
            lastMoveTime = msTicks;
        }
        if(KEY_ReadRight() && player.x < GAME_WIDTH - 1) {
            player.x++;
            lastMoveTime = msTicks;
        }
    }
    
    // 发射子弹
    static uint32_t lastFireTime = 0;
    if(KEY_ReadFire() && (msTicks - lastFireTime) > 300) {
        for(uint8_t i = 0; i < 2; i++) {
            if(!bullets[i].active) {
                bullets[i].x = player.x;
                bullets[i].y = player.y - 1;
                bullets[i].active = 1;
                lastFireTime = msTicks;
                break;
            }
        }
    }
}

static void UpdateBullets(void) {
    for(uint8_t i = 0; i < 2; i++) {
        if(bullets[i].active) {
            bullets[i].y--;
            if(bullets[i].y >= GAME_HEIGHT) { // 超出屏幕
                bullets[i].active = 0;
            }
        }
    }
}

static void UpdateEnemies(void) {
    uint32_t currentTime = msTicks;
    
    // 生成敌机
    if((currentTime - lastEnemySpawn) > 1500 && gameState == GAME_PLAYING) {
        for(uint8_t i = 0; i < 3; i++) {
            if(!enemies[i].active) {
                enemies[i].x = rand() % GAME_WIDTH;
                enemies[i].y = 0;
                enemies[i].active = 1;
                lastEnemySpawn = currentTime;
                break;
            }
        }
    }
    
    // 更新敌机位置
    for(uint8_t i = 0; i < 3; i++) {
        if(enemies[i].active) {
            enemies[i].y++;
            if(enemies[i].y >= GAME_HEIGHT) {
                enemies[i].active = 0;
            }
        }
    }
}

static uint8_t CheckCollisions(void) {
    // 子弹与敌机碰撞
    for(uint8_t i = 0; i < 2; i++) {
        if(bullets[i].active) {
            for(uint8_t j = 0; j < 3; j++) {
                if(enemies[j].active) {
                    if(bullets[i].x == enemies[j].x && bullets[i].y == enemies[j].y) {
                        bullets[i].active = 0;
                        enemies[j].active = 0;
                        score += 10;
                    }
                }
            }
        }
    }
    
    // 玩家与敌机碰撞
    for(uint8_t i = 0; i < 3; i++) {
        if(enemies[i].active) {
            if(player.x == enemies[i].x && player.y == enemies[i].y) {
                return 1; // 游戏结束
            }
        }
    }
    
    return 0;
}

static void RenderGame(void) {
    ClearGameScreen();
    
    // 绘制玩家
    gameScreen[player.y][player.x] = 'A';
    
    // 绘制子弹
    for(uint8_t i = 0; i < 2; i++) {
        if(bullets[i].active && bullets[i].y < GAME_HEIGHT) {
            gameScreen[bullets[i].y][bullets[i].x] = '|';
        }
    }
    
    // 绘制敌机
    for(uint8_t i = 0; i < 3; i++) {
        if(enemies[i].active && enemies[i].y < GAME_HEIGHT) {
            gameScreen[enemies[i].y][enemies[i].x] = '*';
        }
    }
}

void GAME_Update(void) {
    switch(gameState) {
        case GAME_MENU:
            if(KEY_ReadFire()) {
                gameState = GAME_PLAYING;
                GAME_Reset();
                gameState = GAME_PLAYING;
            }
            DISPLAY_ShowMenu(score);
            break;
            
        case GAME_PLAYING:
            UpdatePlayer();
            UpdateBullets();
            UpdateEnemies();
            
            if(CheckCollisions()) {
                gameState = GAME_OVER;
            }
            
            RenderGame();
            DISPLAY_ShowGame(gameScreen, score);
            break;
            
        case GAME_OVER:
            if(KEY_ReadFire()) {
                gameState = GAME_MENU;
            }
            DISPLAY_ShowGameOver(score);
            break;
    }
}
