#include "food.h"

food *InitFood()
{
	srand((unsigned)time(NULL));
	//����һ��ʳ��
	food *f = (food *)malloc(sizeof(food));
	//�������λ��
	f->x = (rand() % 48) * 20 + 20;
	f->y = (rand() % 28) * 20 + 20;
	return f;
}

void ChangeFoodPos(food *f)
{
	srand((unsigned)time(NULL));
	//�������λ��
	f->x = (rand() % 48) * 20 + 20;
	f->y = (rand() % 28) * 20 + 20;
}
