#define START 1
#define GAME 2
#define OVER 3

#include <string.h>
#include <windows.h>
#include "snake.h"
#include "food.h"

// ������Ϣ������
long WINAPI WndProc(HWND hWnd, UINT iMessage, UINT wParam, LONG lParam);
// ��ʼ�������ࣨ���ô������ԣ�
BOOL InitWindowsClass(HINSTANCE hInstance);
// ��ʼ������ (���ƴ���)
BOOL InitWindows(HINSTANCE hInstance, int nCmdShow);
// ����
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
	static CSnake csnake(100, 100);  // ��
	static snake *m_snake = csnake.GetSnake();
	static food *f = InitFood();  // ʳ��

	static int gameState = START; // ��Ϸ״̬

	static HWND button; // ��ʼ��ť
	static int grade = 0; // �ɼ�
	static int level = 1; // �ȼ�(�ƶ��ٶ�)
	static TCHAR fm[] = TEXT("�ɼ�:%d      �ȼ���%d");
	static TCHAR buffere[50];
	
	// ����˫���弼��
	HDC windowDC, bufDC;  // ��Ļ�豸,�����ڴ��豸
	static HDC bgDC, snakeDC, foodDC;  //  ��λͼ�ڴ��豸

	// ���豸��ص�λͼ
	static HBITMAP bufBMP;
	static HBITMAP bgBMP;
	static HBITMAP snakeBMP;
	static HBITMAP foodBMP;

	HBRUSH hBrush; // ��һ����ˢ(���)
	HPEN hPen; // ��һ������(����)
	PAINTSTRUCT PtStr; // ���ƴ�������ṹ��

	switch (iMessage) {
	case WM_CREATE:
		// ����һ��button��Ĵ���--����ť����ô�����ģ����Ա���Ҳ��һ�����ڣ�
		button = CreateWindow(TEXT("button"), TEXT("��ʼ��Ϸ"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			10, 10, 100, 30, hWnd, (HMENU)1, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		// ����λͼ�ļ�
		bgBMP = (HBITMAP)LoadImage(NULL, TEXT("./picture/bg.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		snakeBMP = (HBITMAP)LoadImage(NULL, TEXT("./picture/snake.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		foodBMP = (HBITMAP)LoadImage(NULL, TEXT("./picture/food.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		windowDC = GetDC(hWnd); // ��Ļ�豸�봰�ڹ���
		// ʹ���豸����Ļ�豸����
		bgDC = CreateCompatibleDC(windowDC);
		SelectObject(bgDC, bgBMP);
		snakeDC = CreateCompatibleDC(windowDC);
		SelectObject(snakeDC, snakeBMP);
		foodDC = CreateCompatibleDC(windowDC);
		SelectObject(foodDC, foodBMP);
		DeleteDC(windowDC);
		return 0;
	case WM_PAINT:
		windowDC = BeginPaint(hWnd, &PtStr); // ��Ļ�豸�봰�ڹ���
		// ʹ�����豸����Ļ�豸����
		bufDC = CreateCompatibleDC(windowDC);
		bufBMP = CreateCompatibleBitmap(windowDC, 1000, 600);
		SelectObject(bufDC, bufBMP);

		switch (gameState) {
		case START:
			break;
		case GAME:
			// ������
			BitBlt(bufDC, 0, 0, 1000, 600, bgDC, 0, 0, SRCCOPY);
			// ����
			DrawSnake(m_snake, bufDC, snakeDC);
			// ��ʳ��(͸������)
			BitBlt(bufDC, f->x, f->y, 20, 20, foodDC, 20, 0, SRCPAINT); // ������
			BitBlt(bufDC, f->x, f->y, 20, 20, foodDC, 0, 0, SRCAND);   // ������
			// �������Ļ
			BitBlt(windowDC, 0, 50, 1000, 600, bufDC, 0, 0, SRCCOPY);
			break;
		case OVER:
			//����Ϸ�����Ļ���
			hPen = CreatePen(NULL, NULL, RGB(255, 0, 0));
			hBrush = CreateSolidBrush(RGB(255, 111, 111));
			SetTextColor(windowDC, RGB(255, 0, 0));
			SetBkColor(windowDC, RGB(111, 111, 111));
			SelectObject(windowDC, hPen);
			SelectObject(windowDC, hBrush);
			TextOut(windowDC, 450, 270, TEXT("GAME OVER"), 9);
			break;
		}
		//����ʾ��
		hPen = CreatePen(NULL, NULL, RGB(111, 111, 111));
		hBrush = CreateSolidBrush(RGB(111, 111, 111));
		SelectObject(windowDC, hPen);
		SelectObject(windowDC, hBrush);
		Rectangle(windowDC, 0, 0, 1000, 50);
		//���ɼ��͵ȼ�
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
		//�ߵ��ƶ�
		csnake.Move();
		m_snake = csnake.GetSnake();
		//�ж����Ƿ�Ե�ʳ��
		if (m_snake->x == f->x && m_snake->y == f->y) {
			++grade;
			csnake.AddBody();
			ChangeFoodPos(f);
		}
		//�жϵȼ�
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
		//�ж����Ƿ�ײǽ��\�Լ�����
		if (m_snake->x > 960 || m_snake->x < 20 || m_snake->y > 560 || m_snake->y <20
			|| csnake.IsHeadTouchBody()) {
			KillTimer(hWnd, 1);
			gameState = OVER;
		}
		//�ػ�
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
			csnake.Restart(100, 100);  // ֱ�����¿�ʼ(�����ǵ�һ�λ��ǵ�n��)
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
	// ����ͷ
	int nowShowState = 20 * (int)now_snake->showState;
	BitBlt(bufDC, now_snake->x, now_snake->y, 20, 20, snakeDC, nowShowState, 20, SRCPAINT); // ������
	BitBlt(bufDC, now_snake->x, now_snake->y, 20, 20, snakeDC, nowShowState, 0, SRCAND);   // ������
	// ������
	now_snake = m_snake->next;
	while (now_snake->next) {
		nowShowState = 20 * (int)now_snake->showState;
		if (nowShowState <= 60) {
			BitBlt(bufDC, now_snake->x, now_snake->y, 20, 20, snakeDC, nowShowState, 60, SRCPAINT); // ������
			BitBlt(bufDC, now_snake->x, now_snake->y, 20, 20, snakeDC, nowShowState, 40, SRCAND);   // ������
		}
		else {
			nowShowState -= 80;
			BitBlt(bufDC, now_snake->x, now_snake->y, 20, 20, snakeDC, nowShowState, 100, SRCPAINT); // ������
			BitBlt(bufDC, now_snake->x, now_snake->y, 20, 20, snakeDC, nowShowState, 80, SRCAND);   // ������
		}
		now_snake = now_snake->next;
	}

	// ����β
	nowShowState = 20 * now_snake->last->moveState;
	BitBlt(bufDC, now_snake->x, now_snake->y, 20, 20, snakeDC, nowShowState, 140, SRCPAINT); // ������
	BitBlt(bufDC, now_snake->x, now_snake->y, 20, 20, snakeDC, nowShowState, 120, SRCAND);   // ������
}
