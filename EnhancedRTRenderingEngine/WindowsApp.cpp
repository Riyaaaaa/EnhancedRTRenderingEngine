#include "Windowsx.h"
#include "WindowsApp.h"
#include "resource.h"

#include <iostream>
#include <boost/timer.hpp>
#include <boost/format.hpp>

#include "Common/Defines.h"
#include "Common/Common.h"
#include "Structure/Structure.h"
#include "System/Application.h"
#include "UserData/UserConfig.h"

#include "test.h"

static Size2Dd FixClientSize(HWND hWnd, LONG sx, LONG sy);
static bool ProcessInput(HWND hWnd, unsigned int uMsg, WPARAM wParam, LPARAM lParam);

#define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM                MyRegisterClass(HINSTANCE hInstance);
HWND                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

ERTRESystem::Application* app;

struct InputContext {
	InputKey handleKey;
	Index oldClickedPos;
} iContext;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_ENHANCEDRTRENDERINGENGINE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	HWND hWnd;

	// アプリケーションの初期化を実行します:
	if (!(hWnd = InitInstance(hInstance, nCmdShow)))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ENHANCEDRTRENDERINGENGINE));

#ifdef HAS_UNIT_TEST
	test();
	return 0;
#else

	MSG msg;

	auto windowSize = Size2Dd(1280, 720);
	app = ERTRESystem::Application::getInstance();
	FixClientSize(hWnd, windowSize.w, windowSize.h);

	app->Setup(hWnd);

	bool running = true;
	float elapsed = 0.0f;
	while (running) {
		boost::timer timer;

		app->BeginInput();
		while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) {
				running = false;
			}
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		app->EndInput();

		app->Update(elapsed);

		elapsed = timer.elapsed();
	}

	UserConfig::getInstance()->Flush();

	return (int)msg.wParam;
#endif
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_ENHANCEDRTRENDERINGENGINE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_ENHANCEDRTRENDERINGENGINE);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // グローバル変数にインスタンス処理を格納します。

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return nullptr;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウの描画
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 選択されたメニューの解析:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: HDC を使用する描画コードをここに追加してください...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		ProcessInput(hWnd, uMsg, wParam, lParam);
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
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

bool ProcessInput(HWND hWnd, unsigned int uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_LBUTTONDOWN: {
		Index pos = Index(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		iContext.handleKey = InputKey::LMOUSE;
		iContext.oldClickedPos = pos;
		app->DispatchInputEvent(InputEvent::PRESS, InputKey::LMOUSE, pos);
		SetCapture(hWnd);
	}
						 break;
	case WM_MOUSEMOVE: {
		if (iContext.handleKey != InputKey::None) {
			Index pos = Index{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			app->DispathDragEvent(iContext.handleKey, iContext.oldClickedPos - pos, pos);
			iContext.oldClickedPos = pos;
		}
	}
					   break;
	case WM_LBUTTONUP: {
		Index pos = Index(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		iContext.handleKey = InputKey::None;
		iContext.oldClickedPos = Index{ -1, -1 };
		app->DispatchInputEvent(InputEvent::RELEASE, InputKey::LMOUSE, pos);
		ReleaseCapture();
	}
					   break;
	case WM_RBUTTONDOWN: {
		Index pos = Index(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		iContext.handleKey = InputKey::RMOUSE;
		iContext.oldClickedPos = pos;
		app->DispatchInputEvent(InputEvent::PRESS, InputKey::RMOUSE, pos);
		SetCapture(hWnd);
	}
						 break;
	case WM_RBUTTONUP: {
		Index pos = Index(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		iContext.handleKey = InputKey::None;
		iContext.oldClickedPos = Index{ -1, -1 };
		app->DispatchInputEvent(InputEvent::RELEASE, InputKey::RMOUSE, pos);
		ReleaseCapture();
	}
					   break;
	case WM_CHAR:
		if (L'e' == (wchar_t)wParam) {
			app->DispatchInputEvent(InputEvent::PRESS, InputKey::Up, boost::none);
		}
		else if (L'q' == (wchar_t)wParam) {
			app->DispatchInputEvent(InputEvent::PRESS, InputKey::Down, boost::none);
		}
		else if (L'w' == (wchar_t)wParam) {
			app->DispatchInputEvent(InputEvent::PRESS, InputKey::Forward, boost::none);
		}
		else if (L's' == (wchar_t)wParam) {
			app->DispatchInputEvent(InputEvent::PRESS, InputKey::Backward, boost::none);
		}
		else if (L'a' == (wchar_t)wParam) {
			app->DispatchInputEvent(InputEvent::PRESS, InputKey::Left, boost::none);
		}
		else if (L'd' == (wchar_t)wParam) {
			app->DispatchInputEvent(InputEvent::PRESS, InputKey::Right, boost::none);
		}
		return true;
	}

	return false;
}


static Size2Dd FixClientSize(HWND hWnd, LONG sx, LONG sy)
{
	Size2Dd fixedSize;
	RECT rc1;
	RECT rc2;

	GetWindowRect(hWnd, &rc1);
	GetClientRect(hWnd, &rc2);

	fixedSize.w = ((rc1.right - rc1.left) - (rc2.right - rc2.left));
	fixedSize.h = ((rc1.bottom - rc1.top) - (rc2.bottom - rc2.top));
	sx += fixedSize.w;
	sy += fixedSize.h;
	SetWindowPos(hWnd, NULL, 0, 0, sx, sy, (SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE));

	return fixedSize;
}
