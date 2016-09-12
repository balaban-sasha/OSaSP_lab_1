// lab_1.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "lab_1.h"
#include <string>
#include<vector>
#include<list>	
#include<iostream>
#include<conio.h>
#include "Shape.h"
#include <list>
#include<memory>
#include "fabric.h"
#include <string>
#define MAX_LOADSTRING 100
using namespace std;

void test(HWND hWnd)
{

	MessageBox(hWnd, L"dsadsad", L"dsadsa", 321);
}
std::list <Shapes*> shapesList;
int eqw = 312312;
HWND button1;
int what_need_to_paint;//1-прямая,2-Карандаш,3-Ломаная,4-Прямоугольник,5-Эллипс,6-Многоугольник,7-Текст

std::string checkedShape="pan";
#define ID_COMBODAY 2
// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void CreateButton(HWND hWnd, HWND button1, int x, int y, int cx, int cy, HMENU hmenu, LPCWSTR str)
{
	button1 = CreateWindow(L"button", str, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y, cx, cy, hWnd, (HMENU)hmenu, (HINSTANCE)GetWindowLong(hWnd, GWLP_HINSTANCE), NULL);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: разместите код здесь.

	// Инициализация глобальных строк
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_LAB_1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB_1));

	MSG msg;

	// Цикл основного сообщения:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = 0/*CS_HREDRAW | CS_VREDRAW*/;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB_1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = NULL/*(HBRUSH)(COLOR_WINDOW + 1)*/;
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB_1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	what_need_to_paint = 2;
	hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

	HWND hWnd = /*CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);*/
		CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT, 0, CW_USEDEFAULT, 
			0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}
	CreateButton(hWnd, button1, 10, 10, 70, 20, (HMENU)1001, L"Прямая");
	CreateButton(hWnd, button1, 90, 10, 80, 20, (HMENU)1002, L"Карандаш");
	CreateButton(hWnd, button1, 180, 10, 70, 20, (HMENU)1003, L"Ломаная");
	CreateButton(hWnd, button1, 260, 10, 120, 20, (HMENU)1004, L"Прямоугольник");
	CreateButton(hWnd, button1, 390, 10, 70, 20, (HMENU)1005, L"Эллипс");
	CreateButton(hWnd, button1, 470, 10, 120, 20, (HMENU)1006, L"Многоугольник");
	CreateButton(hWnd, button1, 600, 10, 70, 20, (HMENU)1007, L"Текст");

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND — обработать меню приложения
//  WM_PAINT — отрисовать главное окно
//  WM_DESTROY — отправить сообщение о выходе и вернуться
//
//
HPEN hPen;
BOOL fDraw = FALSE;
POINT ptPrevious = { 0 };
int points_num = 0;
std::vector<POINT> pt;  Shapes* shape;
GenericObjectFactory<string, Shapes> animalFactory;
PAINTSTRUCT ps;
HDC hdc,hdcMem=0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_ERASEBKGND:
		return 1;
	case WM_CREATE:
			animalFactory.add<MyRectangle>("rectangle");
			animalFactory.add<MyEllipse>("ellipse");
			animalFactory.add<MyLine>("line");
			animalFactory.add<MyPan>("pan");
			SetTimer(hWnd, 1, 55, NULL);
			//hPen = CreatePen(10, 10, BLACKNESS);
			break;
	case WM_TIMER:
		InvalidateRect(hWnd, NULL, true);
		UpdateWindow(hWnd);
		break;
	case WM_INITDIALOG:
		//bRet = TRUE;

		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case 1001:
			what_need_to_paint = 1;
			checkedShape = "line";
			fDraw = FALSE;
			break;
		case 1002:
			what_need_to_paint = 2;
			checkedShape = "pan";
			fDraw = FALSE;
			break;
		case 1003:
			what_need_to_paint = 3;
			fDraw = FALSE;
			break;
		case 1004:
			what_need_to_paint = 4;
			checkedShape = "rectangle";
			fDraw = FALSE;
			break;
		case 1005:
			what_need_to_paint = 5;
			checkedShape = "ellipse";
			fDraw = FALSE;
			break;
		case 1006:
			what_need_to_paint = 6;
			fDraw = FALSE;
			break;
		case 1007:
			what_need_to_paint = 7;

			fDraw = FALSE;
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_LBUTTONDOWN:
		shape = animalFactory.get(checkedShape)();
		if (what_need_to_paint != 2) {
			shape->SetStartPosition(LOWORD(lParam), HIWORD(lParam));
			shape->SetEndPosition(LOWORD(lParam), HIWORD(lParam));
		}
		else
		{
			shape->AddPointToArray(LOWORD(lParam), HIWORD(lParam));
			shape->Draw(hdc, hWnd);
		}
			shapesList.push_back(shape);
		fDraw = TRUE;
		ptPrevious.x = LOWORD(lParam);
		ptPrevious.y = HIWORD(lParam);
		break;
	case WM_LBUTTONUP:
		switch (what_need_to_paint)
		{
		/*case 1:if (fDraw)
		{
			hdc = GetDC(hWnd);
			MoveToEx(hdc, ptPrevious.x, ptPrevious.y, NULL);
			LineTo(hdc, LOWORD(lParam), HIWORD(lParam));
			ReleaseDC(hWnd, hdc);
			fDraw = FALSE;
		}
			   break;*/
		/*case 2:if (fDraw)
		{
			hdc = GetDC(hWnd);
			MoveToEx(hdc, ptPrevious.x, ptPrevious.y, NULL);
			LineTo(hdc, LOWORD(lParam), HIWORD(lParam));
			ReleaseDC(hWnd, hdc);
			fDraw = FALSE;
		}
			   break;
	/*	case 4:if (fDraw)
		{
			DeleteObject(&shape);
			Rectangle(hdc, ptPrevious.x, ptPrevious.y, LOWORD(lParam), HIWORD(lParam));
			ReleaseDC(hWnd, hdc);
			fDraw = FALSE;
		}
			   break;
		case 5:if (fDraw)
		{
			DeleteObject(&shape);
			Ellipse(hdc, ptPrevious.x, ptPrevious.y, LOWORD(lParam), HIWORD(lParam));
			ReleaseDC(hWnd, hdc);
			fDraw = FALSE;
		}
			   break;*/
		case 6:if (fDraw)
		{
			hdc = GetDC(hWnd);
			POINT temp[1];
			temp[0].x = LOWORD(lParam);
			temp[0].y = HIWORD(lParam);
			pt.push_back(temp[0]);
			points_num++;
			Polygon(hdc, pt.data(), points_num);
			ReleaseDC(hWnd, hdc);
		}
		default:
			DeleteObject(&shape);
			fDraw = FALSE;
			break;
		}
		break;
	case WM_MOUSEMOVE:
		switch (what_need_to_paint)
		{
		case 2:
			if (fDraw)
			{
				shape->AddPointToArray(LOWORD(lParam), HIWORD(lParam));
			}
			break;
		case 6:
			if (fDraw)
			{
				hdc = GetDC(hWnd);
				POINT temp[1];
				temp[0].x = LOWORD(lParam);
				temp[0].y = HIWORD(lParam);
				Polygon(hdc, pt.data(), points_num);
				ReleaseDC(hWnd, hdc);
			}
			break;
		default:if (fDraw)
		{
			hdc = GetDC(hWnd);
			shape->SetEndPosition(LOWORD(lParam), HIWORD(lParam));
			shape->Draw(hdc, hWnd);
			ReleaseDC(hWnd, hdc);
			break;
		}
		}

		break;

	case WM_PAINT:
	{PAINTSTRUCT ps;
	HDC hdc, hdcOld;
	hdc = BeginPaint(hWnd, &ps);
	hdcOld = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc,
			,
		ps.rcPaint.bottom - ps.rcPaint.top);
	SelectObject(hdcOld, hBitmap);

	SetBkMode(hdcOld, TRANSPARENT);
	FillRect(hdcOld, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
	SelectObject(hdc, hPen);
	for (auto shapeInList : shapesList)
	{
		shapeInList->Draw(hdcOld, hWnd);
	}
	 BitBlt(hdc, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top, hdcOld, 0, 0, SRCCOPY);

	SelectObject(hdcMem, hdcOld);
	DeleteDC(hdcOld);	
	DeleteObject(hBitmap);
	EndPaint(hWnd, &ps);
	}
		break;
	case WM_RBUTTONDOWN:
		hdc = GetDC(hWnd);
		Polygon(hdc, pt.data(), points_num);
		ReleaseDC(hWnd, hdc);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		KillTimer(hWnd, 1);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:

		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
