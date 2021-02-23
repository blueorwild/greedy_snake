#ifndef _GREED_SNAKE_FOOD
#define _GREED_SNAKE_FOOD

#include <stdlib.h>
#include <time.h>

typedef struct food
{
	int x;   // 左上逻辑x坐标
	int y;   // 左上逻辑y坐标
}food;
// 初始化食物
food *InitFood();
// 改变食物位置--位置限制在游戏区域内，不考虑是否跟蛇身体重合
void ChangeFoodPos(food *f);

#endif