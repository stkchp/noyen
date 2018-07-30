#include "stdafx.h"

#include "nyhook.h"

HHOOK hook_proc = NULL;
HINSTANCE dll_module = NULL;
HWND wnd = NULL;

BOOL APIENTRY DllMain(HMODULE _module, DWORD _ul_reason, LPVOID _reserved)
{
	switch (_ul_reason)
	{
	case DLL_PROCESS_ATTACH:
		dll_module = (HINSTANCE)_module;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

LRESULT CALLBACK keyboard_proc(int _code, WPARAM _wparam, LPARAM _lparam)
{
	if (_code != HC_ACTION)
		return CallNextHookEx(hook_proc, _code, _wparam, _lparam);

	KBDLLHOOKSTRUCT *kbd = (KBDLLHOOKSTRUCT *)_lparam;

	// from: vocode:231,scancode:165
	// to:   vkcode:220,scancode:125
	if (_wparam == WM_KEYDOWN)
	{
		if (kbd->vkCode != VK_PACKET) goto end;
		if (kbd->scanCode != 165) goto end;
		keybd_event(220, 125, 0, 0);
		return 1;
	}
	else if (_wparam == WM_KEYUP)
	{
		if (kbd->vkCode != VK_PACKET) goto end;
		if (kbd->scanCode != 165) goto end;
		keybd_event(220, 125, KEYEVENTF_KEYUP, 0);
		return 1;
	}

end:
	return CallNextHookEx(hook_proc, _code, _wparam, _lparam);
}

NYHOOK_API BOOL nyhook_add(HWND _wnd)
{
	wnd = _wnd;
	if (hook_proc) return TRUE;
	hook_proc = SetWindowsHookExW(WH_KEYBOARD_LL, (HOOKPROC)keyboard_proc, dll_module, 0);
	return hook_proc ? TRUE : FALSE;
}

NYHOOK_API BOOL nyhook_remove()
{
	if (!hook_proc) return TRUE;

	if (UnhookWindowsHookEx(hook_proc))
	{
		hook_proc = NULL;
		return TRUE;
	}

	return FALSE;
}
