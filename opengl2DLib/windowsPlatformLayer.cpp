#include "windowsPlatformLayer.h"

const char *windClassAtom = "fereastra";
bool bShouldClose = false;
//todo create a class

namespace platform
{

	namespace internal
	{
		LRESULT CALLBACK windProc(HWND wind, UINT msg, WPARAM wp, LPARAM lp)
		{
			LRESULT rez = 0;
				
			switch (msg)
			{
			case WM_CLOSE:
				bShouldClose = true;
				break;
			default:
				rez = DefWindowProc(wind, msg, wp, lp);
				break;
			}

			return rez;
		}

	};

	Window createWindow(int w, int h, const char* title)
	{

		HINSTANCE hinst = GetModuleHandle(nullptr);

		WNDCLASS wc = {};
		wc.hCursor = LoadCursor(0, IDC_ARROW);
		wc.hInstance = hinst;
		wc.lpfnWndProc = internal::windProc;
		wc.lpszClassName = windClassAtom;
		wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

		RegisterClass(&wc);

		HWND wind = CreateWindow
		(
			windClassAtom,
			title,
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			w,
			h,
			NULL,
			NULL,
			hinst,
			0
		);
		
		Window window;
		window.handle = wind;

		return window;
	}

	void handleEvents(Window wind)
	{
		MSG msg = {};
		while (PeekMessage(&msg, wind.handle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void enableOpengl(Window &wind)
	{
		PIXELFORMATDESCRIPTOR pfd = {};
		int format = 0;

		wind.hdc = GetDC(wind.handle);

		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 8;
		pfd.iLayerType = PFD_MAIN_PLANE;

		format = ChoosePixelFormat(wind.hdc, &pfd);

		SetPixelFormat(wind.hdc, format, &pfd);

		wind.hrc = wglCreateContext(wind.hdc);

		wglMakeCurrent(wind.hdc, wind.hrc);
	}

	bool shouldClose()
	{
		return bShouldClose;
	}

	glm::ivec2 Window::getSize()
	{
		RECT r = {};
		glm::ivec2 size = {};

		GetClientRect(handle, &r);
		size.x = r.right;
		size.y = r.bottom;

		return size;
	}

	void Window::swapBuffers()
	{
		SwapBuffers(hdc);
	}

};

