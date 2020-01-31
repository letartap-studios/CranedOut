#include "gameLayer.h"
#include "windowsPlatformLayer.h"
#include <sstream>
#include <Windows.h>


gl2d::Font f;
gl2d::Texture floorTexture;
gl2d::FrameBuffer fb;

bool initGame(gl2d::Renderer2D &renderer)
{
	f.createFromFile("roboto_black.ttf");
	floorTexture.loadFromFile("floor.png");
	

	return true;
}

int gameWith = 1000;
int gameHeigth = 600;

glm::vec2 players[1] = {};
int playerSize = 50;

bool gameLoop(float deltaTime, gl2d::Renderer2D &renderer, int w, int h, platform::Window &wind)
{
	renderer.clearScreen();

	//renderer.renderRectangle({ 100, 350, 200, 100 }, { 0,0 }, 0, texture);

	//renderer.renderRectangle({ 10,10, 100, 100 }, Colors_Orange, {}, 30);

	//std::cout << platform::joyStickY() << "\n";

	renderer.currentCamera.position.y = (gameHeigth-h)/2.f;
	renderer.currentCamera.position.x = (gameWith-w)/2.f;

	renderer.currentCamera.zoom = platform::getPlayerMovement(0).y * 2 + 1;

	//renderer.currentCamera.position.y += platform::getPlayerMovement(0).y * deltaTime * -200;
	//renderer.currentCamera.position.x += platform::getPlayerMovement(0).x * deltaTime * 200;

	renderer.renderText({ 0,100 }, std::to_string(1.f/deltaTime).c_str(), f, Colors_Red);
	
#pragma region map
	renderer.render9Patch2({ 0, gameHeigth, gameWith, 200 }, 5, Colors_White, { 0,0 }, 0, floorTexture, DefaultTextureCoords, { 0,0.4,0.4,0 });
	renderer.render9Patch2({ -100,0, 100, gameHeigth + 200 }, 5, Colors_White, { 0,0 }, 0, floorTexture, DefaultTextureCoords, { 0,0.4,0.4,0 });
	renderer.render9Patch2({ gameWith ,0, 100, gameHeigth + 200 }, 5, Colors_White, { 0,0 }, 0, floorTexture, DefaultTextureCoords, { 0,0.4,0.4,0 });

#pragma endregion



	renderer.flush();

	return true;
}

void closeGame()
{
	//save files.
	//we don't have to clear resources like textures since os does that for us :P
}
