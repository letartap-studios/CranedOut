#pragma once
#include <Windows.h>
#include "glm/vec2.hpp"

namespace platform
{
	namespace internal
	{
		LRESULT CALLBACK windProc(HWND wind, UINT msg, WPARAM wp, LPARAM lp);
	};

	struct Window
	{
		HWND handle = 0;
		HDC hdc = 0;
		HGLRC hrc = 0;

		glm::ivec2 getSize();
		int getSizeX() { return getSize().x; }
		int getSizeY() { return getSize().y; }
		void swapBuffers();

	};


	//use 'a' for characters of VK_SPACE for other keys
	bool isKeyPressed(int key);

	Window createWindow(int w, int h, const char * title);
	void handleEvents(Window wind);
	void enableOpengl(Window &wind);
	bool shouldClose();
};
