#pragma once
#include <Windows.h>

namespace platform
{
	namespace internal
	{
		LRESULT CALLBACK windProc(HWND wind, UINT msg, WPARAM wp, LPARAM lp);
	};

	HWND createWindow(int w, int h, const char * title);
	void handleEvents(HWND wind);
	void enableOpengl(HWND wind, HDC * hdc, HGLRC * hrc);
	bool shouldClose();
};
