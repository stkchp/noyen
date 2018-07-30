#include "stdafx.h"
#include "noyen.h"
#include "../nyhook/nyhook.h"


#define MID_EXIT 0


HINSTANCE instance = NULL;
static const WCHAR window_title[] = L"noyen";
static const WCHAR window_class[] = L"noyenmainwindow";


LRESULT CALLBACK window_proc(HWND, UINT, WPARAM, LPARAM);


static ATOM _register_class()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = window_proc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = instance;
	wcex.hIcon = LoadIconW(instance, MAKEINTRESOURCEW(IDI_NOYEN));
	wcex.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = window_class;
	wcex.hIconSm = LoadIconW(instance, MAKEINTRESOURCEW(IDI_SMALL));

	return RegisterClassExW(&wcex);
}


static BOOL _init_instance(int _cmdshow)
{
	HWND window = CreateWindowExW(WS_EX_TOOLWINDOW, window_class, window_title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, instance, NULL);

	return window ? TRUE: FALSE;
}


static UINT _tasktray_add(NOTIFYICONDATAW *_nid, HWND _window)
{
	_nid->cbSize = sizeof(NOTIFYICONDATAW);
	_nid->uFlags = (NIF_ICON | NIF_MESSAGE | NIF_TIP);
	_nid->hWnd = _window;
	_nid->hIcon = LoadIconW(instance, MAKEINTRESOURCEW(IDI_NOYEN));
	_nid->uID = 1;
	_nid->uCallbackMessage = WM_TASKTRAY;
	_nid->uTimeout = 10000;
	_nid->dwState = NIS_HIDDEN;
	lstrcpyW(_nid->szTip, L"noyen");

	Shell_NotifyIconW(NIM_ADD, _nid);

	return RegisterWindowMessageW(L"TaskbarCreated");
}


static void _tasktray_reload(NOTIFYICONDATAW *_nid)
{
	Shell_NotifyIconW(NIM_DELETE, _nid);
	Shell_NotifyIconW(NIM_ADD, _nid);
}


static void _tasktray_remove(NOTIFYICONDATAW *_nid)
{
	Shell_NotifyIconW(NIM_DELETE, _nid);
}


static void _menu_create(HWND _window)
{
	HMENU menu;
	POINT pt;

	menu = CreatePopupMenu();

	MENUITEMINFO mInfo = { 0 };

	mInfo.cbSize = sizeof(MENUITEMINFO);
	mInfo.fMask = MIIM_ID | MIIM_STATE | MIIM_STRING;
	mInfo.wID = MID_EXIT;
	mInfo.dwTypeData = L"e&xit";
	InsertMenuItem(menu, -1, TRUE, &mInfo);

	GetCursorPos(&pt);
	SetForegroundWindow(_window);
	TrackPopupMenu(menu, TPM_BOTTOMALIGN, pt.x, pt.y, 0, _window, NULL);

	DestroyMenu(menu);
}


LRESULT CALLBACK window_proc(HWND _window, UINT _message, WPARAM _wparam, LPARAM _lparam)
{
	static NOTIFYICONDATAW nid = { 0 };
	static UINT wm_taskcreate = 0;

	switch (_message)
	{
	case WM_CREATE:
		if (!nyhook_add(_window))
		{
			MessageBoxW(_window, L"hook_add failed!", L"noyen", MB_OK);
			DestroyWindow(_window);
			break;
		}
		wm_taskcreate = _tasktray_add(&nid, _window);
		break;

	case WM_COMMAND:
		if (HIWORD(_wparam) == 0 && LOWORD(_wparam) == MID_EXIT && _lparam == 0)
		{
			DestroyWindow(_window);
		}
		break;

	case WM_TASKTRAY:
		switch (_lparam)
		{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
			_menu_create(_window);
			break;
		}
		break;

	case WM_DESTROY:
		
		_tasktray_remove(&nid);

		if (!nyhook_remove())
		{
			MessageBoxW(_window, L"hook_remove failed!", L"noyen", MB_OK);
		}
		PostQuitMessage(0);
		break;

	default:
		if (_message == wm_taskcreate)
		{
			_tasktray_reload(&nid);
			break;
		}
		return DefWindowProc(_window, _message, _wparam, _lparam);
	}
	return 0;
}


int APIENTRY wWinMain(HINSTANCE _instance, HINSTANCE _prev_instance, LPWSTR _cmdline, int _cmdshow)
{
	UNREFERENCED_PARAMETER(_prev_instance);
	UNREFERENCED_PARAMETER(_cmdline);

	instance = _instance;

	_register_class();

	if (!_init_instance(_cmdshow))
	{
		return FALSE;
	}

	MSG msg;

	while (GetMessageW(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return (int)msg.wParam;
}
