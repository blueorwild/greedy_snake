#define FREE(p) free(p);p=NULL

#include "Snake.h"

CSnake::CSnake(int x, int y)
{
	Initial(x, y);
}

CSnake::~CSnake()
{
	//Destory();
}

void CSnake::Initial(int x, int y)
{
	this->m_direct = M_DOWN;

	this->m_snake = (snake*)malloc(sizeof(snake)); // �½���ͷ
	this->m_snake->x = x;
	this->m_snake->y = y;
	this->m_snake->moveState = M_DOWN; // ��ʼ����
	this->m_snake->showState = S_DOWN_DOWN;
	this->m_snake->next = NULL;
	this->m_snake->last = NULL;

	this->m_move_distance = 10;

	AddBody();  // �������
	AddBody();  // ���β��
}

void CSnake::Destory()
{
	snake *cur_node = this->m_snake;
	snake *next_node;
	while (cur_node->next) {
		next_node = cur_node->next;
		FREE(cur_node);
		cur_node = next_node;
	}
	FREE(cur_node);
}

void CSnake::Restart(int x, int y)
{
	Destory();
	Initial(x, y);
}

void CSnake::Move()
{
	// ���������ͷ,ɾ����β�Ĺ���

	// ��ͷ
	AddBody();
	// ȥβ
	snake *tmp_node = this->m_snake;
	while (tmp_node->next) tmp_node = tmp_node->next;
	tmp_node->last->next = NULL;
	FREE(tmp_node);
}

void CSnake::AddBody()
{
	// �½�һ����ͷ,����ͷ��Ϊ��һ������ڵ�,�������ǰ�˶�����һ��,ˢ����ʾ״̬
	// ���磬����ͷ�����£���ǰ��������ô��ʾ״̬��S_DOWN_DOWN->S_DOWN_LEFT

	snake *new_node = (snake *)malloc(sizeof(snake));
	switch (this->m_direct) {
	case M_UP:
		new_node->x = this->m_snake->x;
		new_node->y = this->m_snake->y - 20;
		if (this->m_snake->moveState == M_LEFT) this->m_snake->showState = S_DOWN_RIGHT;
		else if (this->m_snake->moveState == M_RIGHT) this->m_snake->showState = S_RIGHT_UP;
		break;
	case M_DOWN:
		new_node->x = this->m_snake->x;
		new_node->y = this->m_snake->y + 20;
		if (this->m_snake->moveState == M_LEFT) this->m_snake->showState = S_LEFT_DOWN;
		else if (this->m_snake->moveState == M_RIGHT) this->m_snake->showState = S_UP_LEFT;
		break;
	case M_LEFT:
		new_node->x = this->m_snake->x - 20;
		new_node->y = this->m_snake->y;
		if (this->m_snake->moveState == M_UP) this->m_snake->showState = S_UP_LEFT;
		else if (this->m_snake->moveState == M_DOWN) this->m_snake->showState = S_RIGHT_UP;
		break;
	case M_RIGHT:
		new_node->x = this->m_snake->x + 20;
		new_node->y = this->m_snake->y;
		if (this->m_snake->moveState == M_UP) this->m_snake->showState = S_LEFT_DOWN;
		else if (this->m_snake->moveState == M_DOWN) this->m_snake->showState = S_DOWN_RIGHT;
		break;
	}

	new_node->moveState = this->m_direct;
	new_node->showState = (ShowState)this->m_direct;

	new_node->next = this->m_snake;
	this->m_snake->last = new_node;
	this->m_snake = new_node;
	this->m_snake->last = NULL;
}

bool CSnake::IsHeadTouchBody()
{
	int x = this->m_snake->x;
	int y = this->m_snake->y;

	snake *tmp_node = this->m_snake;
	while (tmp_node = tmp_node->next) 
		if (x == tmp_node->x && y == tmp_node->y) return true;
	return false;
}

void CSnake::ChangeDirect(MoveState direct)
{
	// �ı䷽��������ֻ��Ϊ�Ƕ�����
	if (this->m_direct != (MoveState)(3 - direct))
		this->m_direct = direct;
}

void CSnake::SetMoveDistance(int move_distance)
{
	this->m_move_distance = move_distance;
}

snake *CSnake::GetSnake()
{
	return this->m_snake;
}
