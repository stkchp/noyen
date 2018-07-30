#pragma once

#ifdef NYHOOK_EXPORTS
#define NYHOOK_API __declspec(dllexport)
#else
#define NYHOOK_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

// global hook add/remove
NYHOOK_API BOOL nyhook_add(HWND _wnd);
NYHOOK_API BOOL nyhook_remove();

#ifdef __cplusplus
}
#endif
