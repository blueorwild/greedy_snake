#ifndef _GREED_SNAKE_FOOD
#define _GREED_SNAKE_FOOD

#include <stdlib.h>
#include <time.h>

typedef struct food
{
	int x;   // �����߼�x����
	int y;   // �����߼�y����
}food;
// ��ʼ��ʳ��
food *InitFood();
// �ı�ʳ��λ��--λ����������Ϸ�����ڣ��������Ƿ���������غ�
void ChangeFoodPos(food *f);

#endif