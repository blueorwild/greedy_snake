#include "food.h"

food *InitFood()
{
	srand((unsigned)time(NULL));
	//创建一个食物
	food *f = (food *)malloc(sizeof(food));
	//产生随机位置
	f->x = (rand() % 48) * 20 + 20;
	f->y = (rand() % 28) * 20 + 20;
	return f;
}

void ChangeFoodPos(food *f)
{
	srand((unsigned)time(NULL));
	//产生随机位置
	f->x = (rand() % 48) * 20 + 20;
	f->y = (rand() % 28) * 20 + 20;
}
