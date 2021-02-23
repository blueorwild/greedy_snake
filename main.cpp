#define START 1
#define GAME 2
#define OVER 3

#include <string.h>
#include <windows.h>
#include "snake.h"
#include "food.h"

// 窗口消息处理函数
long WINAPI WndProc(HWND hWnd, UINT iMessage, UINT wParam, LONG lParam);
// 初始化窗口类（设置窗口属性）
BOOL InitWindowsClass(HINSTANCE hInstance);
// 初始化窗口 (绘制窗口)
BOOL InitWindows(HINSTANCE hInstance, int nCmdShow);
// 画蛇
void DrawSnake(snake *m_snake, HDC bfDC, HDC snakeDC);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int nCmdShow)
{
	MSG Message;
	if (!InitWindowsClass(hInstance)) return FALSE;
	if (!InitWindows(hInstance, nCmdShow)) return FALSE;
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

long WINAPI WndProc(HWND hWnd, UINT iMessage, UINT wParam, LONG lParam)
{
	static CSnake csnake(100, 100);  // 蛇
	static snake *m_snake = csnake.GetSnake();
	static food *f = InitFood();  // 食物

	static int gameState = START; // 游戏状态

	static HWND button; // 开始按钮
	static int grade = 0; // 成绩
	static int level = 1; // 等级(移动速度)
	static TCHAR fm[] = TEXT("成绩:%d      等级：%d");
	static TCHAR buffere[50];
	
	// 采用双缓冲技术
	HDC windowDC, bufDC;  // 屏幕设备,缓冲内存设备
	static HDC bgDC, snakeDC, foodDC;  //  各位图内存设备

	// 各设备相关的位图
	static HBITMAP bufBMP;
	static HBITMAP bgBMP;
	static HBITMAP snakeBMP;
	static HBITMAP foodBMP;

	HBRUSH hBrush; // 建一个画刷(填充)
	HPEN hPen; // 建一个画笔(线条)
	PAINTSTRUCT PtStr; // 绘制窗口所需结构体

	switch (iMessage) {
	case WM_CREATE:
		// 创建一个button类的窗口--即按钮，这么创建的？所以本质也是一个窗口？
		button = CreateWindow(TEXT("button"), TEXT("开始游戏"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			10, 10, 100, 30, hWnd, (HMENU)1, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		// 加载位图文件
		bgBMP = (HBITMAP)LoadImage(NULL, TEXT("./picture/bg.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		snakeBMP = (HBITMAP)LoadImage(NULL, TEXT("./picture/snake.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		foodBMP = (HBITMAP)LoadImage(NULL, TEXT("./picture/food.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		windowDC = GetDC(hWnd); // 屏幕设备与窗口关联
		// 使各设备与屏幕设备兼容
		bgDC = CreateCompatibleDC(windowDC);
		SelectObject(bgDC, bgBMP);
		snakeDC = CreateCompatibleDC(windowDC);
		SelectObject(snakeDC, snakeBMP);
		foodDC = CreateCompatibleDC(windowDC);
		SelectObject(foodDC, foodBMP);
		DeleteDC(windowDC);
		return 0;
	case WM_PAINT:
		windowDC = BeginPaint(hWnd, &PtStr); // 屏幕设备与窗口关联
		// 使缓冲设备与屏幕设备兼容
		bufDC = CreateCompatibleDC(windowDC);
		bufBMP = CreateCompatibleBitmap(windowDC, 1000, 600);
		SelectObject(bufDC, bufBMP);

		switch (gameState) {
		case START:
			break;
		case GAME:
			// 画背景
			BitBlt(bufDC, 0, 0, 1000, 600, bgDC, 0, 0, SRCCOPY);
			// 画蛇
			DrawSnake(m_snake, bufDC, snakeDC);
			// 画食物(透明画法)
			BitBlt(bufDC, f->x, f->y, 20, 20, foodDC, 20, 0, SRCPAINT); // 或运算
			BitBlt(bufDC, f->x, f->y, 20, 20, foodDC, 0, 0, SRCAND);   // 与运算
			// 输出到屏幕
			BitBlt(windowDC, 0, 50, 1000, 600, bufDC, 0, 0, SRCCOPY);
			break;
		case OVER:
			//画游戏结束的画面
			hPen = CreatePen(NULL, NULL, RGB(255, 0, 0));
			hBrush = CreateSolidBrush(RGB(255, 111, 111));
			SetTextColor(windowDC, RGB(255, 0, 0));
			SetBkColor(windowDC, RGB(111, 111, 111));
			SelectObject(windowDC, hPen);
			SelectObject(windowDC, hBrush);
			TextOut(windowDC, 450, 270, TEXT("GAME OVER"), 9);
			break;
		}
		//画显示区
		hPen = CreatePen(NULL, NULL, RGB(111, 111, 111));
		hBrush = CreateSolidBrush(RGB(111, 111, 111));
		SelectObject(windowDC, hPen);
		SelectObject(windowDC, hBrush);
		Rectangle(windowDC, 0, 0, 1000, 50);
		//画成绩和等级
		SetTextColor(windowDC, RGB(255, 0, 0));
		SetBkColor(windowDC, RGB(111, 111, 111));
		SelectObject(windowDC, hPen);
		SelectObject(windowDC, hBrush);
		TextOut(windowDC, 150, 20, buffere, wsprintf(buffere, fm, grade, level));

		DeleteDC(bufDC);
		EndPaint(hWnd, &PtStr);
		return 0;
	case WM_TIMER:
		if (gameState == OVER) break;
		//蛇的移动
		csnake.Move();
		m_snake = csnake.GetSnake();
		//判断蛇是否吃到食物
		if (m_snake->x == f->x && m_snake->y == f->y) {
			++grade;
			csnake.AddBody();
			ChangeFoodPos(f);
		}
		//判断等级
		switch (grade) {
		case 1:
			level = 2;
			KillTimer(hWnd, 1);
			SetTimer(hWnd, 1, 250, NULL);
			break;
		case 2:
			level = 3;
			KillTimer(hWnd, 1);
			SetTimer(hWnd, 1, 200, NULL);
			break;
		case 4:
			level = 4;
			KillTimer(hWnd, 1);
			SetTimer(hWnd, 1, 150, NULL);
			break;
		case 8:
			level = 4;
			KillTimer(hWnd, 1);
			SetTimer(hWnd, 1, 100, NULL);
			break;
		case 16:
			level = 5;
			KillTimer(hWnd, 1);
			SetTimer(hWnd, 1, 50, NULL);
			break;
		}
		//判断蛇是否撞墙上\自己身体
		if (m_snake->x > 960 || m_snake->x < 20 || m_snake->y > 560 || m_snake->y <20
			|| csnake.IsHeadTouchBody()) {
			KillTimer(hWnd, 1);
			gameState = OVER;
		}
		//重绘
		InvalidateRect(hWnd, NULL, false);
		return 0;
	case WM_KEYDOWN:
		if (gameState == OVER) break;
		switch (wParam) {
		case VK_UP:
			csnake.ChangeDirect(M_UP);
			break;
		case VK_DOWN:
			csnake.ChangeDirect(M_DOWN);
			break;
		case VK_LEFT:
			csnake.ChangeDirect(M_LEFT);
			break;
		case VK_RIGHT:
			csnake.ChangeDirect(M_RIGHT);
			break;
		}
		return 0;
	case WM_COMMAND:
		switch (wParam) {
		case 1:
			gameState = GAME;
			level = 1;
			grade = 0;
			csnake.Restart(100, 100);  // 直接重新开始(无论是第一次还是第n次)
			SetTimer(hWnd, 1, 300, NULL);
			break;
		}
		SetFocus(hWnd);
		return 0;
	case WM_DESTROY:
		csnake.Destory();
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

BOOL InitWindowsClass(HINSTANCE hInstance)
{
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = TEXT("GameWindow");

	return RegisterClass(&wndclass);
}

BOOL InitWindows(HINSTANCE hInstance, int nCmdShow)
{
	HWND    hwnd;
	hwnd = CreateWindow(TEXT("GameWindow"), TEXT("Snake"), WS_OVERLAPPEDWINDOW^WS_THICKFRAME^WS_MAXIMIZEBOX,
		170, 22, 1015, 688, NULL, NULL, hInstance, NULL);
	if (!hwnd) return FALSE;

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	return TRUE;
}

void DrawSnake(snake *m_snake, HDC bufDC, HDC snakeDC)
{
	snake *now_snake = m_snake;
	// 画蛇头
	int nowShowState = 20 * (int)now_snake->showState;
	BitBlt(bufDC, now_snake->x, now_snake->y, 20, 20, snakeDC, nowShowState, 20, SRCPAINT); // 或运算
	BitBlt(bufDC, now_snake->x, now_snake->y, 20, 20, snakeDC, nowShowState, 0, SRCAND);   // 与运算
	// 画蛇身
	now_snake = m_snake->next;
	while (now_snake->next) {
		nowShowState = 20 * (int)now_snake->showState;
		if (nowShowState <= 60) {
			BitBlt(bufDC, now_snake->x, now_snake->y, 20, 20, snakeDC, nowShowState, 60, SRCPAINT); // 或运算
			BitBlt(bufDC, now_snake->x, now_snake->y, 20, 20, snakeDC, nowShowState, 40, SRCAND);   // 与运算
		}
		else {
			nowShowState -= 80;
			BitBlt(bufDC, now_snake->x, now_snake->y, 20, 20, snakeDC, nowShowState, 100, SRCPAINT); // 或运算
			BitBlt(bufDC, now_snake->x, now_snake->y, 20, 20, snakeDC, nowShowState, 80, SRCAND);   // 与运算
		}
		now_snake = now_snake->next;
	}

	// 画蛇尾
	nowShowState = 20 * now_snake->last->moveState;
	BitBlt(bufDC, now_snake->x, now_snake->y, 20, 20, snakeDC, nowShowState, 140, SRCPAINT); // 或运算
	BitBlt(bufDC, now_snake->x, now_snake->y, 20, 20, snakeDC, nowShowState, 120, SRCAND);   // 与运算
}
