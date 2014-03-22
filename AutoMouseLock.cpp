#include <windows.h>

#ifdef DEBUG
#include <stdio.h>
#endif

BOOL wasFullScreen = FALSE;
int lastCheck = 0;

inline void check()
{
	// Don't check more than once per tick,
	// to lessen CPU load
	int now = GetTickCount();
	if (lastCheck == now)
		return;
	lastCheck = now;

	HWND fg = GetForegroundWindow();
	RECT fgRect;
	GetClientRect(fg, &fgRect);

	int monW = GetSystemMetrics(SM_CXSCREEN);
	int monH = GetSystemMetrics(SM_CYSCREEN);

#ifdef DEBUG
	printf("%d %d %d %d %d %d\n",
		fg,
		fg != GetDesktopWindow(),
		fgRect.left == 0,
		fgRect.top == 0,
		fgRect.right == monW,
		fgRect.bottom == monH
	);

#endif

	BOOL fullScreen =
		fg &&
		fg != GetDesktopWindow() &&
		fgRect.left == 0 &&
		fgRect.top == 0 &&
		fgRect.right == monW &&
		fgRect.bottom == monH;

	if (fullScreen != wasFullScreen)
	{
		#ifdef DEBUG
		printf("%s!\n", fullScreen ? "Locking" : "Unlocking");
		#endif

		BOOL ok = ClipCursor(fullScreen ? &fgRect : NULL);

		#ifdef DEBUG
		printf("%s!\n", ok ? "Success" : "Failed");
		#endif

		wasFullScreen = fullScreen;
	}
}

// Use a low-level mouse hook to get around Windows restrictions
// Method credit to http://stackoverflow.com/a/12027257/21501

HHOOK hMouseHook;

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	check();
	return CallNextHookEx(hMouseHook, nCode, wParam, lParam);
}

void main()
{
	hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, &LowLevelMouseProc, GetModuleHandle(NULL), 0);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnhookWindowsHookEx(hMouseHook);
	ClipCursor(NULL);
}
