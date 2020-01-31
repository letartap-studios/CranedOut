#include "gameLayer.h"
#include "windowsPlatformLayer.h"
#include <sstream>
#include <Windows.h>


gl2d::Font f;
gl2d::Texture texture;
gl2d::FrameBuffer fb;

bool initGame(gl2d::Renderer2D &renderer)
{
	f.createFromFile("roboto_black.ttf");
	texture.loadFromFile("test.jpg");
	
	fb.create(300, 300);
	fb.clear();

	renderer.updateWindowMetrics(300, 300);
	renderer.renderRectangle({ 0, 0,300,300 }, Colors_Magenta, {0,0}, 0);

	renderer.flushFBO(fb);

	return true;
}

bool gameLoop(float deltaTime, gl2d::Renderer2D &renderer, int w, int h, platform::Window &wind)
{
	renderer.clearScreen();

	//renderer.renderRectangle({ 100, 350, 200, 100 }, { 0,0 }, 0, texture);

	//renderer.renderRectangle({ 10,10, 100, 100 }, Colors_Orange, {}, 30);

	std::cout << wind.getRelMousePosition().x << ' ' << wind.getRelMousePosition().y << "\n";


	renderer.renderText({ 0,100 }, std::to_string(1.f/deltaTime).c_str(), f, Colors_Red);
	
	renderer.renderText({ 0,200 }, "text Vlad", f, Colors_Blue);
	renderer.renderText({ 0,300 }, "text Mihai", f, Colors_Green);


	renderer.renderRectangle({ 100,150, 100, 100 }, { 0,0 }, 0, fb.texture);
	//renderer.render9Patch2({ 100,150, 200, 200 }, 10, Colors_White, { 0,0 }, 0, texture, DefaultTextureCoords, { 0,0.8,0.8,0 });

	//renderer.renderRectangle({ 10,10, 100, 100 }, colors, {}, 30);

	renderer.flush();

	return true;
}

void closeGame()
{
	//save files.
	//we don't have to clear resources like textures since os does that for us :P
}
