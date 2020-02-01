#include "windowsPlatformLayer.h"

const char *windClassAtom = "fereastra";
bool bShouldClose = false;
//todo create a class

namespace platform
{
	typedef DWORD WINAPI XInputGetState_t(DWORD dwUserIndex, XINPUT_STATE* pState);
	static XInputGetState_t *DynamicXinputGetState;
	bool xInputLoaded = 0;

	namespace internal
	{
		
		LRESULT CALLBACK windProc(HWND wind, UINT msg, WPARAM wp, LPARAM lp)
		{
			LRESULT rez = 0;
				
			Window *windPtr = (Window*)GetWindowLongPtr(wind, GWLP_USERDATA);

			switch (msg)
			{
			case WM_CLOSE:
				bShouldClose = true;
				break;

			case WM_NCCREATE:
			{
				CREATESTRUCT *cs = (CREATESTRUCT*)lp;
			
				Window *windPtr = ((Window*)cs->lpCreateParams);
			
				SetWindowLongPtr(wind, GWLP_USERDATA, (LONG)windPtr);
			
				rez = DefWindowProc(wind, msg, wp, lp);
				break;
			}
			case WM_RBUTTONDOWN:
				if (windPtr)
					windPtr->rightClick = 1;
				break;
			case WM_RBUTTONUP:
				if (windPtr)
					windPtr->rightReleased= 1;
				break;
			case WM_LBUTTONDOWN:
				if (windPtr)
					windPtr->leftClick = 1;
				break;
			case WM_LBUTTONUP:
				if (windPtr)
					windPtr->leftReleased = 1;
				break;
			//case WM_MOUSELEAVE:
			//	if (windPtr)
			//	{
			//	windPtr->mouseIn = 0;
			//	}
			//	break;
			//case WM_MOUSEHOVER:
			//	if (windPtr)
			//	{
			//		windPtr->mouseIn = 1;
			//	}
			//	break;
			default:
				rez = DefWindowProc(wind, msg, wp, lp);
				break;
			}

			return rez;
		}

	};

	bool isKeyPressed(int key)
	{
		return GetAsyncKeyState(key);
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

	void Window::create(int w, int h, const char* title)
	{

		HINSTANCE hinst = GetModuleHandle(nullptr);

		WNDCLASS wc = {};
		wc.hCursor = LoadCursor(0, IDC_ARROW);
		wc.hInstance = hinst;
		wc.lpfnWndProc = internal::windProc;
		wc.lpszClassName = windClassAtom;
		wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

		RegisterClass(&wc);

		handle = CreateWindow
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
			(LPVOID)this
		);

		//TRACKMOUSEEVENT ev = {};
		//ev.cbSize = sizeof(TRACKMOUSEEVENT);
		//ev.dwFlags = TME_HOVER | TME_LEAVE;
		//ev.hwndTrack = handle;
		//ev.dwHoverTime = HOVER_DEFAULT;
		//TrackMouseEvent(&ev);

		hdc = GetDC(handle);

	}

	void Window::handleEvents(int count)
	{
		MSG msg = {};

		rightClick = 0;
		leftClick = 0;
		leftReleased = 0;
		rightReleased = 0;

		if (count != 0)
		{
			int i = 0;
			while (PeekMessage(&msg, handle, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				i++;
				if (i > count)break;

			}
		}else
		{
			while (PeekMessage(&msg, handle, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		
		
	}

	void Window::enableOpengl()
	{
		PIXELFORMATDESCRIPTOR pfd = {};
		int format = 0;

		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 8;
		pfd.iLayerType = PFD_MAIN_PLANE;

		format = ChoosePixelFormat(hdc, &pfd);

		SetPixelFormat(hdc, format, &pfd);

		hrc = wglCreateContext(hdc);

		wglMakeCurrent(hdc, hrc);
	}

	bool Window::isLeftMouseButtonHeld()
	{
		return ((GetKeyState(VK_LBUTTON) & 0x80) != 0);
	}

	bool Window::isRightMouseButtonHeld()
	{
		return ((GetKeyState(VK_RBUTTON) & 0x80) != 0);
	}

	glm::ivec2 Window::getRelMousePosition()
	{
		POINT p = {};
		GetCursorPos(&p);

		ScreenToClient(handle, &p);

		return { p.x, p.y };
	}

	void loadXinput()
	{
		HMODULE xinputLib = LoadLibrary("xinput1_4.dll");
		if (!xinputLib)
		{
			xinputLib = LoadLibrary("xinput1_3.dll");
		}
		if(!xinputLib)
		{
			return ;
		}

		{
			DynamicXinputGetState = (XInputGetState_t*)GetProcAddress(xinputLib, "XInputGetState");
			xInputLoaded = 1;
		}

		
	}

	const int keyBindings[2][4] = 
	{ {'W', 'A', 'S', 'D'},
	{VK_UP, VK_LEFT, VK_DOWN, VK_RIGHT }
	};

	glm::vec2 getPlayerMovement(int id)
	{
		if(id>1)
		{
			return {};
		}
		int i = id;
		XINPUT_STATE s;

		if (DynamicXinputGetState != nullptr && DynamicXinputGetState(i, &s) == ERROR_SUCCESS)
		{
			XINPUT_GAMEPAD *pad = &s.Gamepad;

			float retValX = pad->sThumbLX / (float)SHRT_MAX;

			retValX = max(-1.f, retValX);
			retValX = min(1.f, retValX);

			if (abs(retValX) < 0.12)
			{
				retValX = 0.f;
			}

			float retValY = pad->sThumbLY / (float)SHRT_MAX;

			retValY = max(-1.f, retValY);
			retValY = min(1.f, retValY);

			if (abs(retValY) < 0.12)
			{
				retValY = 0.f;
			}

			return { retValX, -retValY };
			//bool UP = (pad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
			//return (float)UP;
		}else
		{
			float retValX = 0;
			float retValY = 0;

			if (isKeyPressed(keyBindings[i][0]))
			{
				retValY += 1;
			}

			if (isKeyPressed(keyBindings[i][1]))
			{
				retValX -= 1;
			}

			if (isKeyPressed(keyBindings[i][2]))
			{
				retValY -= 1;
			}

			if (isKeyPressed(keyBindings[i][3]))
			{
				retValX += 1;
			}

			return{ retValX, -retValY };
		
		}
	}

	//deprecated probably
	glm::vec2 joyStick()
	{
		if (DynamicXinputGetState != nullptr)
			for (int i = 0; i < XUSER_MAX_COUNT; i++)
			{
				XINPUT_STATE s;
				if (DynamicXinputGetState(i, &s) == ERROR_SUCCESS)
				{
					XINPUT_GAMEPAD *pad = &s.Gamepad;
					
					float retValX = pad->sThumbLX / (float)SHRT_MAX;

					retValX = max(-1.f, retValX);
					retValX = min(1.f, retValX);

					if (abs(retValX) < 0.12)
					{
						retValX = 0.f;
					}

					float retValY = pad->sThumbLY / (float)SHRT_MAX;

					retValY = max(-1.f, retValY);
					retValY = min(1.f, retValY);

					if(abs(retValY) < 0.12)
					{
						retValY = 0.f;
					}

					return { retValX, retValY };
					//bool UP = (pad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
					//return (float)UP;
				}
				else
				{
					
				}

			}
		return {};
	}

};

