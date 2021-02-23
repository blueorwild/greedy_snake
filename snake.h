#ifndef _GREED_SNAKE_
#define _GREED_SNAKE_

#include <stdlib.h>

/*********************************************************
--���ֻ�������������ĸ�����,�����ƶ�����ת��Ӧ����
--'�d','ʮ'�˸�����,���Է�����
*********************************************************/
// ��ʾ״̬
enum ShowState{
	S_UP_UP, S_LEFT_LEFT, S_RIGHT_RIGHT, S_DOWN_DOWN,  // ʮ
	S_UP_LEFT, S_RIGHT_UP, S_DOWN_RIGHT, S_LEFT_DOWN,    // '�d'���߷���'�d',��ʵһ����
};
// �˶�״̬
enum MoveState {
	M_UP, M_LEFT, M_RIGHT, M_DOWN
};

//�ߵĽڵ�
typedef struct snake
{
	int x; // x����
	int y; // y����
	MoveState moveState; // �˶�״̬
	ShowState showState; // ��ʾ״̬
	snake *next; // ��һ���ڵ�
	snake *last; // ��һ���ڵ�
}snake;

class CSnake
{
	MoveState m_direct; // ��ǰ��ͷ�˶�����
	snake *m_snake;
	int m_move_distance;  //ÿ���ƶ��ľ��룬���Ѷȵ���(Ϊ�˿���ȥ�����˶�����ʱ���������䣬�����Ǿ��벻�䶨ʱ����)  ����ʧ����
public:
	CSnake(int x, int y);
	~CSnake();

	void Initial(int x, int y); // ��ʼ����������
	void Destory(); // ������������
	void Restart(int x, int y); // ��Ϸ�������¿�ʼ(δ�رճ���)

	void Move(); // ���ƶ�
	void AddBody(); // �ߵ���������
	bool IsHeadTouchBody(); // �����ͷ�Ƿ���������
	void ChangeDirect(MoveState direct); // �ı��˶�����
	void SetMoveDistance(int move_distance); // �����ƶ�����
	snake *GetSnake(); // ��ȡ��

};

#endif    //ifndef _GREED_SNAKE_
