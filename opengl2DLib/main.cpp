#include <gl/glew.h>

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <chrono>

#include "opengl2Dlib.h"
#include "windowsPlatformLayer.h"

#include "gameLayer.h"

int WINAPI wWinMain(
	HINSTANCE   hInstance,
	HINSTANCE   hPrevInstance,
	PWSTR       lpCmdLine,
	int         nCmdShow
)
{

#pragma region init
	platform::Window wind(1000, 800, "window");
	
	//enabels opengl for that window
	wind.enableOpengl();

	//after we have a window, we have to enable opengl drivers
	glewInit();

	//initialize the 2d library after we have initialized opengl drivers
	gl2d::init();


	gl2d::Renderer2D renderer;
	renderer.create();
#pragma endregion

	platform::loadXinput();
	//for(int i=0;i<3;i++)
	//{
	//	platform::vibrateBoth(0, 30000);
	//	Sleep(150);
	//	platform::vibrateBoth(0, 0);
	//	Sleep(400);
	//	platform::vibrateBoth(60000, 0);
	//	Sleep(150);
	//	platform::vibrateBoth(0, 0);
	//	Sleep(150);
	//	platform::vibrateBoth(0, 65000);
	//	Sleep(100);
	//	platform::vibrateBoth(0, 0);
	//	platform::vibrateBoth(60000, 60000);
	//	Sleep(400);
	//	platform::vibrateBoth(0, 0);
	//	Sleep(100);
	//	platform::vibrateBoth(0, 60000);
	//	platform::vibrateBoth(0, 0);
	//	Sleep(100);
	//}

	if (!initGame(renderer)) 
	{
		return 0;
	}
	
	using ms = std::chrono::duration<float, std::milli>;

	auto start = std::chrono::high_resolution_clock::now();
	auto stop = std::chrono::high_resolution_clock::now();
	


	while (!platform::shouldClose())
	{

		stop = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration_cast<ms>(stop - start).count();
		start = std::chrono::high_resolution_clock::now();
		deltaTime /= 1000.f;

		wind.handleEvents();

		int w; int h;
		w = wind.getSizeX();
		h = wind.getSizeY();

		renderer.updateWindowMetrics(w, h);

		//run frame
		//todo check delta time and vsync
		if(!gameLoop(deltaTime, renderer, w, h, wind))
		{
			break;
		}

		wind.swapBuffers();
		
		//FlashWindow(wind.handle, 0);
	}

	closeGame();

	return 0;
}