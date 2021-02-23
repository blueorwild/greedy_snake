#ifndef _GREED_SNAKE_
#define _GREED_SNAKE_

#include <stdlib.h>

/*********************************************************
--玩家只考虑上下左右四个方向,但蛇移动考虑转角应包括
--'卍','十'八个方向,所以分两种
*********************************************************/
// 显示状态
enum ShowState{
	S_UP_UP, S_LEFT_LEFT, S_RIGHT_RIGHT, S_DOWN_DOWN,  // 十
	S_UP_LEFT, S_RIGHT_UP, S_DOWN_RIGHT, S_LEFT_DOWN,    // '卍'或者反的'卍',其实一样的
};
// 运动状态
enum MoveState {
	M_UP, M_LEFT, M_RIGHT, M_DOWN
};

//蛇的节点
typedef struct snake
{
	int x; // x坐标
	int y; // y坐标
	MoveState moveState; // 运动状态
	ShowState showState; // 显示状态
	snake *next; // 下一个节点
	snake *last; // 上一个节点
}snake;

class CSnake
{
	MoveState m_direct; // 当前蛇头运动方向
	snake *m_snake;
	int m_move_distance;  //每次移动的距离，随难度递增(为了看上去匀速运动，定时器不变距离变，而不是距离不变定时器变)  ……失败了
public:
	CSnake(int x, int y);
	~CSnake();

	void Initial(int x, int y); // 初始化所有内容
	void Destory(); // 销毁所有内容
	void Restart(int x, int y); // 游戏结束重新开始(未关闭程序)

	void Move(); // 蛇移动
	void AddBody(); // 蛇的身体增长
	bool IsHeadTouchBody(); // 检查蛇头是否碰到身体
	void ChangeDirect(MoveState direct); // 改变运动方向
	void SetMoveDistance(int move_distance); // 设置移动距离
	snake *GetSnake(); // 获取蛇

};

#endif    //ifndef _GREED_SNAKE_
