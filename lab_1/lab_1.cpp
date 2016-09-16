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
#include <commdlg.h>
#include <windows.h>
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
//checkedShape is use that to know what shape we want to draw, default it is pen
std::string checkedShape="pen";
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
INT_PTR CALLBACK    DlgProc(HWND, UINT, WPARAM, LPARAM);


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

	HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT, 0, CW_USEDEFAULT, 
			0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}
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
//initialization of new variables
HPEN hPen;
BOOL fDraw = FALSE;
POINT ptPrevious = { 0 };
int points_num = 0;
std::vector<POINT> pt;  
Shapes* shape;
GenericObjectFactory<string, Shapes> shapesFactory;
PAINTSTRUCT ps;
HDC hdc,hdcMem=0;
COLORREF brush, brushFill=RGB(255,255,255);
int penSize=1;
RECT rec;
HENHMETAFILE hMTF;
static HBRUSH hBrush;
static OPENFILENAME ofn;
static char fullpath[256], filename[256], dir[256];
ENHMETAHEADER emh;
RECT rect, rect1;
float coefficientOfSizeHeight =1, coefficientOfSizeWidth = 1;
int shearX, shearY;
int zDelta;
bool needToFill = false;
bool needToZoom = false;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static PTCHAR text;
	static int size = 0;
	switch (message)
	{
	case WM_KEYDOWN:
		//delete last shape
		if(!shapesList.empty())
		if ((GetAsyncKeyState(VK_CONTROL)) && (GetAsyncKeyState(VK_F1)))
			shapesList.pop_back();
		break;
	case WM_MOUSEWHEEL:
	{
		//use that to manage zoom
		if (needToZoom) {
			if ((GetAsyncKeyState(VK_CONTROL)) && (!GetAsyncKeyState(VK_SHIFT)))
			{
				if ((fDraw) && (what_need_to_paint != 7))
					fDraw = false;
				zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
				if ((zDelta < 0) && (coefficientOfSizeHeight < 3))
					coefficientOfSizeHeight += 0.2;
				else
					if ((zDelta > 0) && (coefficientOfSizeHeight > 0.6))
						coefficientOfSizeHeight -= 0.2;
				needToFill = true;
			}
			if ((GetAsyncKeyState(VK_CONTROL)) && (GetAsyncKeyState(VK_SHIFT)))
			{
				if ((fDraw) && (what_need_to_paint != 7))
					fDraw = false;
				zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
				if ((zDelta < 0) && (coefficientOfSizeWidth < 3))
					coefficientOfSizeWidth += 0.2;
				else
					if ((zDelta > 0) && (coefficientOfSizeWidth > 0.6))
						coefficientOfSizeWidth -= 0.2;
				needToFill = true;
			}
		}
		else
		{
			//use that to manage pen
			if (GetAsyncKeyState(VK_SHIFT))
			{
				if ((fDraw) && (what_need_to_paint != 7))
					fDraw = false;
				zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
				if ((zDelta < 0))
					shearX += 1;
				else
					if ((zDelta > 0))
						shearX -= 1;
				needToFill = true;
			}
			if (!GetAsyncKeyState(VK_SHIFT))
			{
				if ((fDraw) && (what_need_to_paint != 7))
					fDraw = false;
				zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
				if ((zDelta < 0))
					shearY += 1;
				else
					if ((zDelta > 0))
						shearY -= 1;
				needToFill = true;
			}
		}
	}break;

	case WM_ERASEBKGND:
		return 1;
	case WM_CREATE:
			//add shapes to factory
			shapesFactory.add<MyRectangle>("rectangle");
			shapesFactory.add<MyEllipse>("ellipse");
			shapesFactory.add<MyLine>("line");
			shapesFactory.add<MyPen>("pen");
			shapesFactory.add<MyPolyLine>("polyline");
			shapesFactory.add<MyPolygone>("polygone");
			shapesFactory.add<MyText>("text");
			shapesFactory.add<MyEraser>("eraser");

			SetTimer(hWnd, 1, 55, NULL); 
			hPen = CreatePen(BS_SOLID, 1, NULL);
			text = (PTCHAR)GlobalAlloc(GPTR, 50000 * sizeof(TCHAR));
			break;
	case WM_TIMER:
		//that to redraw window
		InvalidateRect(hWnd, NULL, true);
		UpdateWindow(hWnd);
		break;
	case WM_INITDIALOG:
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;

		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
			//8 cases below are use that to choose new shape
		case 32779:
			what_need_to_paint = 1;
			checkedShape = "line";
			fDraw = FALSE;
			break;
		case 32773:
			what_need_to_paint = 2;
			checkedShape = "pen";
			fDraw = FALSE;
			break;
		case 32774:
			what_need_to_paint = 3;
			checkedShape = "polyline";
			fDraw = FALSE;
			break;
		case 32775:
			what_need_to_paint = 4;
			checkedShape = "rectangle";
			fDraw = FALSE;
			break;
	
		case 32776:
			what_need_to_paint = 5;
			checkedShape = "ellipse";
			fDraw = FALSE;
			break;
		case 32777:
			what_need_to_paint = 6;
			checkedShape = "polygone";
			fDraw = FALSE;
			break;
		case 32778:
			what_need_to_paint = 7;
			checkedShape = "text";
			fDraw = FALSE;
			break;
		case 32794:
			what_need_to_paint = 8;
			checkedShape = "eraser";
			fDraw = FALSE;
			break;
			//next 4 cases use that to create new pen by new pen size
		case 32789:
			penSize = 1;
			hPen = CreatePen(BS_SOLID, 1, brush);
			break;
		case 32790:
			penSize = 2;
			hPen = CreatePen(BS_SOLID, 2, brush);
			break;
		case 32791:
			penSize = 4;
			hPen = CreatePen(BS_SOLID, 4, brush);
			break;
		case 32792:
			penSize = 6;
			hPen = CreatePen(BS_SOLID, 6, brush);
			break;
			//that to choose  pen color
		case 32780:
			CHOOSECOLOR cc;
			COLORREF crCustColors[16];
			cc.lStructSize = sizeof(CHOOSECOLOR);
			cc.hwndOwner = NULL;
			cc.hInstance = NULL;
			cc.rgbResult = brush;
			cc.lpCustColors = crCustColors;
			cc.Flags = CC_RGBINIT | CC_FULLOPEN;
			cc.lCustData = 0L;
			cc.lpfnHook = NULL;
			cc.lpTemplateName = NULL;
			if (ChooseColor(&cc))
				brush=cc.rgbResult;
			hPen = CreatePen(BS_SOLID, penSize, brush);
			break;
			//that to choose fill for shapes
		case 32793:
			cc.lStructSize = sizeof(CHOOSECOLOR);
			cc.hwndOwner = NULL;
			cc.hInstance = NULL;
			cc.rgbResult = brushFill;
			cc.lpCustColors = crCustColors;
			cc.Flags = CC_RGBINIT | CC_FULLOPEN;
			cc.lCustData = 0L;
			cc.lpfnHook = NULL;
			cc.lpTemplateName = NULL;
			if (ChooseColor(&cc))
				brushFill = cc.rgbResult;
			break;
			//use that to save file
		case 32771:
		{
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;
			ofn.hInstance = hInst; 
			ofn.lpstrFilter = L"Metafile (*.emf)\0*.emf\0Все файлы (*.*)\0*.*\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFile = (LPWSTR)fullpath;
			ofn.nMaxFile = sizeof(fullpath);
			ofn.lpstrFileTitle = (LPWSTR)filename;
			ofn.nMaxFileTitle = sizeof(filename);
			ofn.lpstrInitialDir = (LPWSTR)dir;
			ofn.lpstrTitle = L"Save file as...";
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_EXPLORER;
			if (GetSaveFileName(&ofn))
			{
				HDC hdc = CreateEnhMetaFile(NULL, (LPCWSTR)fullpath, NULL, NULL);

				if (hMTF)
				{
					RECT rect;
					GetClientRect(hWnd, &rect);
					PlayEnhMetaFile(hdc, hMTF, &rect);
				}

				for (auto shapeInList : shapesList)
				{
					shapeInList->Draw(hdc);
				}

				DeleteEnhMetaFile(CloseEnhMetaFile(hdc));
			}
		}break;
		//use that to open file
		case 32772:
		{
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;
			ofn.hInstance = hInst; 
			ofn.lpstrFilter = L"Metafile (*.emf)\0*.emf\0Все файлы (*.*)\0*.*\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFile = (LPWSTR)fullpath;
			ofn.nMaxFile = sizeof(fullpath);
			ofn.lpstrFileTitle = (LPWSTR)filename;
			ofn.nMaxFileTitle = sizeof(filename);
			ofn.lpstrInitialDir = (LPWSTR)dir;
			ofn.lpstrTitle = L"Save file as...";
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_EXPLORER;
			if (GetOpenFileName(&ofn))
			{
				HDC hdc = CreateEnhMetaFile(NULL, NULL, NULL, NULL);
				hMTF = GetEnhMetaFile((LPWSTR)fullpath);
				shapesList.clear();
				if (hMTF)
				{
					RECT rect;
					GetClientRect(hWnd, &rect);
					PlayEnhMetaFile(hdc, hMTF, &rect);
				}

			
			}
		}break;
		case ID_PRINT:
			//use that to print all shapes
		{
			PRINTDLG pd;
			HBITMAP      hbmMem;
			HANDLE       hOld;
			static HDC hBitmapDC = 0;
			ZeroMemory(&pd, sizeof(pd));
			pd.lStructSize = sizeof(pd);
			pd.hwndOwner = hWnd;
			pd.hDevMode = NULL; 
			pd.hDevNames = NULL; 
			pd.Flags = PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC;
			pd.nCopies = 1;
			pd.nFromPage = 0xFFFF;
			pd.nToPage = 0xFFFF;
			pd.nMinPage = 1;
			pd.nMaxPage = 0xFFFF;


			if (PrintDlg(&pd) == TRUE)
			{
				

				DOCINFO  document_Information;
				HDC hPrinter = pd.hDC;

				memset(&document_Information, 0, sizeof(document_Information));
				document_Information.cbSize = sizeof(document_Information);
				StartDoc(hPrinter, &document_Information);

				StartPage(hPrinter);
				if (hMTF)
				{
					RECT rect;
					GetClientRect(hWnd, &rect);
					PlayEnhMetaFile(hPrinter, hMTF, &rect);
				}
				for (auto shapeInList : shapesList)
				{
					shapeInList->Draw(hPrinter);
				}
				EndPage(hPrinter);

				EndDoc(hPrinter);
				DeleteDC(hPrinter);
			}
		}break;
		//if we check zoom
		case 32799:
		{
			needToZoom = true;
		}break;
		//if we check pen
		case 32800:
		{
			needToZoom = false;
		}break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	//use for text
	case WM_CHAR:
		switch (wParam)
		{
		case VK_RETURN:
			size = 0;
			break;
		case VK_SPACE:
			size = 0; break;
		default:
				text[size] = (char)wParam;
				size++;
				if(shape!=NULL)
				shape->SetText(wParam, size);
			break;
		}
		//if l button down need to create new shape
	case WM_LBUTTONDOWN:
		if(fDraw)
			if (what_need_to_paint==7)
			{
				fDraw == false;
			}
		if (!fDraw) {
			if ((what_need_to_paint != 2) && (what_need_to_paint != 6)&& (what_need_to_paint != 3) && (what_need_to_paint != 7) && (what_need_to_paint != 8)) {
				shape = shapesFactory.get(checkedShape)();
				shape->SetStartPosition(LOWORD(lParam), HIWORD(lParam));
				shape->SetEndPosition(LOWORD(lParam), HIWORD(lParam));
			}
			else if(what_need_to_paint != 7)
			{
				shape = shapesFactory.get(checkedShape)();
				shape->AddPointToArray(LOWORD(lParam), HIWORD(lParam));
			}
			else
			{
				shape = shapesFactory.get(checkedShape)();
				shape->SetStartPosition(LOWORD(lParam), HIWORD(lParam));
			}
			if (what_need_to_paint != 8)
				shape->SetHpen(hPen);
			else
				shape->SetHpen(CreatePen(BS_SOLID, penSize, RGB(255, 255, 255)));
			
				shape->SetBrush(brushFill);
				fDraw = TRUE;
			shapesList.push_back(shape);
		}
		
		break;
		//add new point for some shapes if need
	case WM_LBUTTONUP:
		SetRect(&rec, LOWORD(lParam), HIWORD(lParam), LOWORD(lParam) + 200, HIWORD(lParam) + 200);
		switch (what_need_to_paint)
		{
		case 3:case 6:if (fDraw)
		{
			shape->AddPointToArray(LOWORD(lParam), HIWORD(lParam));
		}
			   break;
		case 7:{	shape->SetStartPosition(LOWORD(lParam), HIWORD(lParam)); 
}
			   break;
		default:
			DeleteObject(&shape);
			fDraw = FALSE;
			break;
		}
		break;
		//change last position for shapes
	case WM_MOUSEMOVE:
		switch (what_need_to_paint)
		{
			case 2:case 8:
			if (fDraw)
			{
				shape->AddPointToArray(LOWORD(lParam), HIWORD(lParam));
			}
			break;
		case 3:case 6:
			if (fDraw)
			{
				
			}
			break;
		default:if (fDraw)
		{
			shape->SetEndPosition(LOWORD(lParam), HIWORD(lParam));
			break;
		}
		}
		break;
		//load all shapes on device context with double buffing
	case WM_PAINT:
	{
	PAINTSTRUCT ps;
	HDC hdc, hdcOld;
	hdc = BeginPaint(hWnd, &ps);
	RECT rect;
	GetClientRect(hWnd, &rect);
	hdcOld = CreateCompatibleDC(hdc); 
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc,
		ps.rcPaint.right - ps.rcPaint.left,
		ps.rcPaint.bottom - ps.rcPaint.top);
	SelectObject(hdcOld, hBitmap);

	SetBkMode(hdcOld, TRANSPARENT);
	FillRect(hdcOld, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

	SelectObject(hdc, hPen);

	GetClientRect(hWnd, &rect);
	if (hMTF)
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		rect.left = 0;
		rect.top = 0;
		rect.right = 1500;
		rect.bottom = 700;
		PlayEnhMetaFile(hdcOld, hMTF, &rect);
	}
	for (auto shapeInList : shapesList)
	{
		shapeInList->Draw(hdcOld);
	}
	if (needToFill)
		FillRect(hdc, &rect, WHITE_BRUSH);
		StretchBlt(hdc, ps.rcPaint.left, ps.rcPaint.top, (ps.rcPaint.right - ps.rcPaint.left) / coefficientOfSizeWidth, (ps.rcPaint.bottom - ps.rcPaint.top) / coefficientOfSizeHeight, hdcOld, 0 + shearX, 0 + shearY, (ps.rcPaint.right - ps.rcPaint.left), (ps.rcPaint.bottom - ps.rcPaint.top), SRCCOPY);
	
	needToFill = false; 
	SelectObject(hdcMem, hdcOld);
	DeleteDC(hdcOld);	
	DeleteObject(hBitmap);
	EndPaint(hWnd, &ps);
	}
		break;
	//use for text,polygone,polyline that to set end of shape drawing
	case WM_RBUTTONDOWN: {
		DeleteObject(shape);
		size = 0;
		fDraw = false;
	}
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