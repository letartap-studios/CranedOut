#include "gameLayer.h"
#include "windowsPlatformLayer.h"
#include <sstream>
#include <Windows.h>
#include "Animate.h"
#include "Rectangle.h"
#include <algorithm>

gl2d::Font f;
gl2d::Texture floorTexture;
gl2d::Texture gearTexture;
gl2d::Texture animTexture;
gl2d::Texture backgroundTexture[1];
Animate anim;
bool pickedUp = false;


RectangleBody podea, cub, crane;

bool initGame(gl2d::Renderer2D &renderer)
{
	f.createFromFile("roboto_black.ttf");
	floorTexture.loadFromFile("floor.png");
	gearTexture.loadFromFile("gear.png");

	InitPhysics();

	podea.Create(0, 600, 1000, 100, 0.1);
	podea.body->enabled = false;

	cub.Create(50, 200, 100, 100, 0.1);
	crane.Create(150, 0, 20, 20, 0.1, 3000);
	SetPhysicsGravity(0, 1);
	
	animTexture.loadFromFile("rotita.png");
	backgroundTexture[0].loadFromFile("background.png");
	backgroundTexture[1].loadFromFile("background2.png");

	anim.create(4, 1, 50, animTexture);

	return true;
}

int gameWith = 800;
int gameHeigth = 700;

glm::vec2 players[2] = { {100, 100}, {400, 100} };
int playerSize = 40;

bool gameLoop(float deltaTime, gl2d::Renderer2D &renderer, int w, int h, platform::Window &wind)
{
	renderer.clearScreen();

	float velocity = -0.8 * deltaTime;

	
	RunPhysicsStep();
	
#pragma region determinePlacement
	renderer.currentCamera.position.y = (gameHeigth - h) / 2.f;
	renderer.currentCamera.position.x = (gameWith - w) / 2.f;

	{
		float zoom = 1;
		if (gameWith < w)
		{
			zoom = (w) / (gameWith * 1.9);
		}
		else if (gameWith > w)
		{
			zoom = (w) / (gameWith * 0.9);
		}
		
		float Hzoom = 1;
		if (gameHeigth < h)
		{
			Hzoom =(float) (h) / (gameHeigth * 2.0);
		}
		else if (gameHeigth > h)
		{
			Hzoom = (h) / (gameHeigth * 0.9);
		}

		if(gameWith*1.1 < w || gameHeigth*1.1 < h)
		{
			zoom = (w) / (gameWith * 1);
			Hzoom = (h) / (gameHeigth * 1);
			zoom = min(zoom, Hzoom);
		}else
		{
			zoom = min(zoom, Hzoom);
		}

		zoom = min(zoom, 3);
		zoom = max(zoom, 0.1);

		renderer.currentCamera.zoom = zoom;
		//renderer.currentCamera.zoom = platform::getPlayerMovement(0).y * 1 + 1;
	}
	
#pragma endregion

#pragma region background

	{
		int xSize = backgroundTexture[0].GetSize().x;
		int ySize = backgroundTexture[0].GetSize().y;
		
		glm::vec2 pos = renderer.currentCamera.position;

		//pos.x += xSize / 2;
		//pos.y += ySize / 2;

		float darkness = ((sin(GetTickCount() / 1000.f) + 1) / 2.f) / 2.f + 0.2f;

		renderer.renderRectangle({ pos.x, pos.y, xSize, ySize }, { darkness,darkness,darkness,1}, {}, 0, backgroundTexture[0]);
		
	}

#pragma endregion
	
	//renderer.currentCamera.position.y += platform::getPlayerMovement(0).y * deltaTime * -200;
	//renderer.currentCamera.position.x += platform::getPlayerMovement(0).x * deltaTime * 200;

	///fps
	//renderer.renderText({ 0,100 }, std::to_string(1.f/deltaTime).c_str(), f, Colors_Red);
	
#pragma region player
	players[0] += platform::getPlayerMovement(0) * deltaTime * 200.f;
	players[1] += platform::getPlayerMovement(1) * deltaTime * 200.f;

	for(int i=0; i<2; i++)
	{
		if (players[i].x - (playerSize / 2) < 0)
		{
			players[i].x = playerSize / 2;
		}

		if (players[i].y - playerSize / 2 < 0)
		{
			players[i].y = playerSize / 2;
		}

		if (players[i].x + playerSize / 2 > gameWith)
		{
			players[i].x = gameWith - playerSize/2;
		}

		if (players[i].y + playerSize / 2 > gameHeigth)
		{
			players[i].y = gameHeigth - playerSize / 2;
		}
	}

	anim.updateTime(deltaTime * 1000);
	renderer.renderRectangle({ players[0].x - playerSize / 2, players[0].y - playerSize / 2 , playerSize , playerSize }, {}, 0, animTexture, anim.getTexturePos());
	renderer.renderRectangle({ players[1].x - playerSize / 2, players[1].y - playerSize / 2 , playerSize , playerSize }, {}, 0, gearTexture);

	float wireLength0 = glm::distance(crane.getPos(), players[0]);
	float wireLength1 = glm::distance(crane.getPos(), players[1]);
	//auto wireLength = sqrt(pow(static_cast<float>(crane.body->position.x - players[0].x), 2) - pow(static_cast<float>(crane.body->position.y - players[0].y),2));
	float clampValue = 0.15;

	if (wireLength0 >= -35 && wireLength0 <= -40)
	{
		crane.body->velocity.y = -getGravity().y * deltaTime;
	}
	
	if(wireLength0 > 300)
	{
		PhysicsAddForce(crane.body, {(crane.getPos().x - players[0].x) * velocity, (crane.getPos().y - players[0].y) * velocity });
		PhysicsAddForce(crane.body, {(crane.getPos().x - players[1].x) * velocity, (crane.getPos().y - players[1].y) * velocity });
		//crane.body->velocity = { (crane.body->position.x - players[0].x) * velocity, (crane.body->position.y - players[0].y) * velocity };

	}

	if (crane.body->velocity.x < 0)
	{
		crane.body->velocity.x = max(crane.body->velocity.x, -clampValue);
	}
	if (crane.body->velocity.y < 0)
	{
		crane.body->velocity.y = max(crane.body->velocity.y, -clampValue);
	}

	if (crane.body->velocity.x > 0)
	{
		crane.body->velocity.x = min(crane.body->velocity.x, clampValue);
	}
	if (crane.body->velocity.y > 0)
	{
		crane.body->velocity.y = min(crane.body->velocity.y, clampValue);
	}

	std::cout<<crane.body->velocity.y<< std::endl;
	if (cub.PointCollision(crane.getPos()))
	{
		if(platform::isKeyPressed(VK_SPACE))
		{
			if(pickedUp == false)
			{
				glm::vec2 temp = crane.getPos() - glm::vec2{ cub.body->position.x, cub.body->position.y };
				pickedUp = true;
			}
			cub.body->useGravity = false;
			cub.body->position.x = crane.getPos().x;
			cub.body->position.y = crane.getPos().y;
		}
		else
		{
			cub.body->useGravity = true;
			pickedUp = false;
			//	cub.body->enabled = true;
		}
		
	}
	
#pragma endregion

#pragma region map

	for(int i=0; i<=gameHeigth/100; i++)
	{
		renderer.renderRectangle({ -100, i * 100, 100, 100 }, {}, 0, floorTexture);
		renderer.renderRectangle({ gameWith ,i * 100 , 100, 100 }, {}, 0, floorTexture);
	}

	for(int i=0; i<gameWith/100;i++)
	{
		renderer.renderRectangle({ i * 100, gameHeigth, 100, 100}, {}, 0, floorTexture);
	}

	//renderer.render9Patch2({ 0, gameHeigth, gameWith, 200 }, 5, Colors_White, { 0,0 }, 0, floorTexture, DefaultTextureCoords, { 0,0.4,0.4,0 });
	//renderer.render9Patch2({ -100,0, 100, gameHeigth + 200 }, 5, Colors_White, { 0,0 }, 0, floorTexture, DefaultTextureCoords, { 0,0.4,0.4,0 });
	//renderer.render9Patch2({ gameWith ,0, 100, gameHeigth + 200 }, 5, Colors_White, { 0,0 }, 0, floorTexture, DefaultTextureCoords, { 0,0.4,0.4,0 });
#pragma endregion
	podea.Draw(Colors_Red, renderer);
	cub.Draw(Colors_Magenta, renderer);
	crane.Draw(gearTexture, renderer);

	
	renderer.flush();

	return true;
}

void closeGame()
{
	//save files.
	//we don't have to clear resources like textures since os does that for us :P
}
