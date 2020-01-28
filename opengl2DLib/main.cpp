#include <gl/glew.h>

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <chrono>

#include "opengl2Dlib.h"
#include "windowsPlatformLayer.h"

#include "gameLayer.h"

int main() 
{

#pragma region init
	auto wind = platform::createWindow(620, 420, "window");

	//enabels opengl for that window
	platform::enableOpengl(wind);

	//after we have a window, we have to enable opengl drivers
	glewInit();

	//initialize the 2d library after we have initialized opengl drivers
	gl2d::init();

	gl2d::Renderer2D renderer;
	renderer.create();
#pragma endregion

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

		platform::handleEvents(wind);

		int w; int h;
		w = wind.getSizeX();
		h = wind.getSizeY();

		renderer.updateWindowMetrics(w, h);

		//run frame
		//todo check delta time and vsync
		if(!gameLoop(deltaTime, renderer, w, h))
		{
			break;
		}

		wind.swapBuffers();
	}

	closeGame();

	return 0;
}