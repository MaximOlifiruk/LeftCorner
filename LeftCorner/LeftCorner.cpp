// LeftCorner.cpp : Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "LeftCorner.h"
#include <Windowsx.h>
#include <stdio.h>
#include <string>
#include <Windows.h>
#include <commctrl.h>
#include <commdlg.h>

#define STRICT
#define MAX_LOADSTRING 100
#define WM_LBUTTONDBLCLK                0x0203
#define WM_LBUTTONDOWN                  0x0201
#define WM_RBUTTONDOWN                  0x0205

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LEFTCORNER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LEFTCORNER));

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

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LEFTCORNER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LEFTCORNER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


static int X;
static int Y;
static DWORD rgbCurrent;
static COLORREF col;

BOOL __stdcall DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
	{

		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			BOOL bSuccess;
			X = GetDlgItemInt(hWnd, IDC_EDIT1, &bSuccess, FALSE);
			Y = GetDlgItemInt(hWnd, IDC_EDIT2, &bSuccess, FALSE);
			EndDialog(hWnd, 0);
		}
	}
	}
	return 0;
}




//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
void func(HWND hWnd) {
	HDC hdc = GetDC(hWnd);
	SelectObject(hdc, CreatePen(PS_SOLID, 1, rgbCurrent));
	RECT rc;
	char buf[80];
	GetWindowRect(hWnd, &rc);
	sprintf_s(buf, "x: %i\n"
		" y: %i\n", rc.left,rc.top );
	const WCHAR* pwcsName;
	int size = MultiByteToWideChar(CP_ACP, 0, buf, -1, NULL, 0);
	pwcsName = new WCHAR[50];
	MultiByteToWideChar(CP_ACP, 0, buf, -1, (LPWSTR)pwcsName, size);
	SetTextColor(hdc, col);
	TextOut(hdc, X, Y, pwcsName, size);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT ps;
	HINSTANCE hInstance;
	static COLORREF acrCustClr[16]; // array of custom colors 
	static DWORD rgbCurrent;

    switch (message)
    {


	case WM_LBUTTONDOWN: {
		POINT cursorPos;
		GetCursorPos(&cursorPos);
		ScreenToClient(hWnd, &cursorPos);
		X = cursorPos.x; Y = cursorPos.y;
		func(hWnd);
	}
	 break;

	case WM_RBUTTONDOWN: {
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc);
		func(hWnd);
	}

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;

			case IDM_ABOUT1:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc);
				func(hWnd);
				break;

			case ID_ABOUT2:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc);
				func(hWnd);
				break;

			case ID_32777:
				

			case ID_32779:
				CHOOSECOLOR cc;                 // common dialog box structure 
				static COLORREF acrCustClr[16]; // array of custom colors 
				HBRUSH hbrush;                  // brush handle
				     // initial color selection
					
				// Initialize CHOOSECOLOR 
				ZeroMemory(&cc, sizeof(cc));
				cc.lStructSize = sizeof(cc);
				cc.hwndOwner = hWnd;
				cc.lpCustColors = (LPDWORD)acrCustClr;
				cc.rgbResult = rgbCurrent;
				cc.Flags = CC_FULLOPEN | CC_RGBINIT;
		
				if (ChooseColor(&cc) == TRUE)
				{
					hbrush = CreateSolidBrush(cc.rgbResult);
					rgbCurrent = cc.rgbResult;
				}
				col = rgbCurrent;
			
			
				break;

            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;

            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

			SelectObject(hdc, CreatePen(PS_SOLID, 1, rgbCurrent));
			//TextOut(hdc, 10, 10, L"AAA", 10);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
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

