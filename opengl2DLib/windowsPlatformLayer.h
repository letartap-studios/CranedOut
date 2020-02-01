#pragma once
#include <Windows.h>
#include "glm/vec2.hpp"
#include <Xinput.h>
#include "glm/vec2.hpp"

namespace platform
{
	namespace internal
	{
		LRESULT CALLBACK windProc(HWND wind, UINT msg, WPARAM wp, LPARAM lp);
	};

	struct Window
	{
		Window() {}
		Window(int w, int h, const char* title) { create(w, h, title); }

		Window(const Window &other) = delete;
		Window operator= (const Window &other) = delete;

		HWND handle = 0;
		HDC hdc = 0;
		HGLRC hrc = 0;

		glm::ivec2 getSize();
		int getSizeX() { return getSize().x; }
		int getSizeY() { return getSize().y; }
		void swapBuffers();

		void create(int w, int h, const char* title);
		void handleEvents();
		void enableOpengl();

		bool isLeftMouseButtonHeld();
		bool isRightMouseButtonHeld();

		//WM_LBUTTONDBLCLK	The left mouse button was double - clicked.
		//WM_LBUTTONDOWN	The left mouse button was pressed.
		//WM_LBUTTONUP	The left mouse button was released.
		//WM_MBUTTONDBLCLK	The middle mouse button was double - clicked.
		//WM_MBUTTONDOWN	The middle mouse button was pressed.
		//WM_MBUTTONUP	The middle mouse button was released.
		//WM_RBUTTONDBLCLK	The right mouse button was double - clicked.
		//WM_RBUTTONDOWN	The right mouse button was pressed.
		//WM_RBUTTONUP	The right mouse button was released.

		char leftClick = 0;
		char rightClick = 0;
		char leftReleased = 0;
		char rightReleased = 0;
		//char mouseIn = 0;

		glm::ivec2 getRelMousePosition();
	};

	//use 'A' for characters of VK_SPACE for other keys
	bool isKeyPressed(int key);

	void loadXinput();

	glm::vec2 joyStick();
	glm::vec2 getPlayerMovement(int id);
	bool playerPressesAButton(int id);

	int getPlayerResizeString(int id);

	void vibrate(int id, short l, short r);
	void vibrateBoth(short l, short r);


	bool shouldClose();
};
