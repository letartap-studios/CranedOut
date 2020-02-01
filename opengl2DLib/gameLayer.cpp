#include "gameLayer.h"
#include "windowsPlatformLayer.h"
#include <sstream>
#include <Windows.h>
#include "Animate.h"
#include "Rectangle.h"
#include <algorithm>
#include <vector>
#include "rayfork_audio.h"

//extern rf_audio_context rfAudioContext;

gl2d::Font f;
gl2d::Texture floorTexture;
gl2d::Texture gearTexture;
gl2d::Texture animTexture;
gl2d::Texture backgroundTexture;
gl2d::Texture noapteTexture;
gl2d::Texture stringTexture;
gl2d::Texture macara;
gl2d::Texture topTexture;
gl2d::Texture topTexture2;
gl2d::Texture blockTexture;

Animate playerAnim[2];
bool pickedUp = false;
glm::vec2 tempPadding;

Animate macaraAnim;

RectangleBody podea, crane;
std::vector<RectangleBody> bodies;

//rf_long_audio_stream music;

int gameWith = 900;
int gameHeigth = 700;

float vibrateTime = 0;

bool initGame(gl2d::Renderer2D& renderer)
{
	f.createFromFile("roboto_black.ttf");
	floorTexture.loadFromFile("floor.png");
	gearTexture.loadFromFile("gear.png");
	stringTexture.loadFromFile("string.png");
	macara.loadFromFile("animal_macara.png");
	topTexture.loadFromFile("top.png");
	topTexture2.loadFromFile("top2.png");
	blockTexture.loadFromFile("block.png");

	macaraAnim.create(1, 4, 0, macara);

	InitPhysics();

	podea.Create(0 + gameWith / 2, gameHeigth + 400 / 2, gameWith, 400, 0.1);
	podea.body->enabled = false;

	//cub.Create(50, 200, 100, 100, 0.1);

	bodies.push_back({ 50, 200, 50, 50, 0.3 });
	bodies.push_back({ 50, 300, 50, 50, 0.3 });
	bodies.push_back({ 50, 400, 50, 50, 0.2 });
	bodies.push_back({ 50, 500, 50, 50, 0.2 });
	bodies.push_back({ 250, 500, 100, 50, 0.2 });

	crane.Create(150, 0, 50, 50, 4, 4000);
	crane.body->freezeOrient = true;

	//SetPhysicsGravity(0, 3);

	animTexture.loadFromFile("rotita.png");
	backgroundTexture.loadFromFile("zi.png");
	noapteTexture.loadFromFile("noapte.png");

	playerAnim[0].create(4, 1, 20, animTexture);
	playerAnim[1].create(4, 1, 20, animTexture);


	//rf_audio_init(&rfAudioContext);


	//music = rf_load_long_audio_stream("music.mp3");
	//rf_play_long_audio_stream(music);



	return true;
}


glm::vec2 players[2] = { {100, 100}, {400, 100} };
int playerSize = 40;

const int wireSizeMaxEnarge = 300;
const int wireSizeMinEnarge = 50;

float wireSize[2] = { 250, 250 };
int maxWireSize = 600;
float clampValue = 0.2;

bool gameLoop(float deltaTime, gl2d::Renderer2D& renderer,  int w,  int h,  platform::Window& wind)
{

	//rf_update_long_audio_stream(music);

	renderer.clearScreen();

	float velocity = -1 * deltaTime;

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
			Hzoom = (h) / (gameHeigth * 2.0);
		}
		else if (gameHeigth > h)
		{
			Hzoom = (h) / (gameHeigth * 0.9);
		}

		if (gameWith * 1.1 < w || gameHeigth * 1.1 < h)
		{
			zoom = (w) / (gameWith * 1);
			Hzoom = (h) / (gameHeigth * 1);
			zoom = min(zoom, Hzoom);
		}
		else
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
		int xSize = backgroundTexture.GetSize().x * 1.4;
		int ySize = backgroundTexture.GetSize().y * 1.4;

		glm::vec2 pos = renderer.currentCamera.position;

		//pos.x += xSize / 2;
		//pos.y += ySize / 2;

		float darkness = ((sin(GetTickCount() / 10000.f) + 1) / 2.f) / 2.f + 0.5f;

		float dayFactor = 1;

		if(darkness >= 0.6)
		{
			dayFactor = pow(darkness, 0.9);
		}
		else if (darkness <= 0.3)
		{
			dayFactor = pow(darkness, 2);
		}
		else 
		{ 
			dayFactor = darkness;
		}

		renderer.renderRectangle({ pos.x, pos.y, xSize, ySize }, { darkness,darkness,darkness,dayFactor }, {}, 0, backgroundTexture);
		renderer.renderRectangle({ pos.x, pos.y, xSize, ySize }, { darkness,darkness,darkness,1- dayFactor }, {}, 0, noapteTexture);

	}

#pragma endregion

	//renderer.currentCamera.position.y += platform::getPlayerMovement(0).y * deltaTime * -200;
	//renderer.currentCamera.position.x += platform::getPlayerMovement(0).x * deltaTime * 200;

	///fps
	//renderer.renderText({ 0,100 }, std::to_string(1.f / deltaTime).c_str(), f, Colors_Red);

#pragma region player

	{
		float modify[2] = {};

		if (vibrateTime)
		{
			platform::vibrateBoth(60000, 60000);
			vibrateTime -= deltaTime;
			if (vibrateTime < 0)
			{
				vibrateTime = 0;
				platform::vibrateBoth(0, 0);
			}
		}

		for (int i = 0; i < 2; i++)
		{
			modify[i] = platform::getPlayerResizeString(i);

			modify[i] *= deltaTime * 100;

			wireSize[i] += modify[i];

			wireSize[i] = max(wireSize[i], wireSizeMinEnarge);
			wireSize[i] = min(wireSize[i], wireSizeMaxEnarge);

		}

		bool l = modify[0] != 0;
		bool r = modify[1] != 0;

		if (l)playerAnim[0].updateTime(deltaTime * 1000);
		if (r)playerAnim[1].updateTime(deltaTime * 1000);

		platform::vibrateBoth(12000 * l, 12000 * r);

	}

	players[0] += platform::getPlayerMovement(0) * deltaTime * 200.f;
	players[1] += platform::getPlayerMovement(1) * deltaTime * 200.f;

	for (int i = 0; i < 2; i++)
	{
		if (players[i].x - (playerSize / 2) < -100)
		{
			players[i].x = playerSize / 2 - 100;
		}

		if (players[i].y - playerSize / 2 < 0)
		{
			players[i].y = playerSize / 2;
		}

		if (players[i].x + playerSize / 2 > gameWith + 100)
		{
			players[i].x = gameWith - playerSize / 2 + 100;
		}

		if (players[i].y + playerSize / 2 > gameHeigth- 200)
		{
			players[i].y = gameHeigth - playerSize / 2 - 200;
		}
	}

	for(int i=0; i<players[0].x / 100; i++)
	{
		int padd = (int)players[0].x % 100 - 100;
		renderer.renderRectangle({ i * 100 + padd, -100, 100, 100 }, {}, 0, topTexture2);
	}

	for (int i = players[1].x / 100; i < gameWith / 100 ; i++)
	{
		int padd = (int)players[1].x % 100 ;
		renderer.renderRectangle({ i * 100 + padd, -100, 100, 100 }, {}, 0, topTexture2);
	}

	renderer.renderRectangle({ players[0].x - playerSize / 2, players[0].y - playerSize / 2 , playerSize , playerSize }, Colors_Blue, {}, 0, playerAnim[0].texture, playerAnim[0].getTexturePos());
	renderer.renderRectangle({ players[1].x - playerSize / 2, players[1].y - playerSize / 2 , playerSize , playerSize }, Colors_Red, {}, 0, playerAnim[1].texture, playerAnim[1].getTexturePos());

	float wireLength[2];
	for (int i = 0; i < 2; i++)
	{
		wireLength[i] = glm::distance(crane.getPos(), players[i]);
	}

#pragma region crane

	//auto wireLength = sqrt(pow(static_cast<float>(crane.body->position.x - players[0].x), 2) - pow(static_cast<float>(crane.body->position.y - players[0].y),2));

	for (int i = 0; i < 2; i++)
	{
		//if (wireLength[i] >= -35 && wireLength[i] <= -40)
		//{
		//	crane.body->velocity.y = -getGravity().y * deltaTime;
		//	break;
		//}

		if (wireLength[i] > wireSize[i])
		{
			float magnifier = (wireLength[i] - wireSize[i]) / 2.f;
			PhysicsAddForce(crane.body, { (crane.getPos().x - players[i].x) * velocity * magnifier, (crane.getPos().y - players[i].y) * velocity * magnifier });
			//crane.body->velocity = { (crane.body->position.x - players[0].x) * velocity, (crane.body->position.y - players[0].y) * velocity };

			if (wireLength[i] > maxWireSize)
			{
				glm::vec2 direction = players[i] - crane.getPos();
				direction = glm::normalize(direction);
				direction *= wireLength[i] - maxWireSize;
				crane.body->position.x += direction.x;
				crane.body->position.y += direction.y;
			}
		}
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

	if (crane.getPos().y > gameHeigth)
	{
		crane.setPos({ crane.getPos().x ,gameHeigth });
	}

#pragma endregion

	if (platform::playerPressesAButton(0) && platform::playerPressesAButton(1))
	{

		macaraAnim.posY = 1;
	}
	else if (platform::playerPressesAButton(0))
	{
		macaraAnim.posY = 3;
	}
	else if (platform::playerPressesAButton(1))
	{
		macaraAnim.posY = 2;
	}
	else
	{
		macaraAnim.posY = 0;
	}

	bool anyPickedUp = 0;
	for (auto& cub : bodies)
	{
		if (cub.PointCollision(crane.getPos()))
		{
			if (platform::playerPressesAButton(0) && platform::playerPressesAButton(1))
			{
				anyPickedUp = true;
				if (pickedUp == false)
				{
					vibrateTime = 0.2;

					tempPadding = crane.getPos() - glm::vec2{ cub.body->position.x, cub.body->position.y };
					tempPadding *= 0.5;
					pickedUp = true;
				}
				cub.body->useGravity = false;
				cub.body->position.x = crane.getPos().x - tempPadding.x;
				cub.body->position.y = crane.getPos().y - tempPadding.y;
			}
			else
			{
				cub.body->useGravity = true;
				//	cub.body->enabled = true;
			}

		}
		else
		{
			cub.body->useGravity = true;
			//	cub.body->enabled = true;
		}
	}
	if (anyPickedUp == false)
	{
		pickedUp = false;
	}

#pragma endregion


#pragma region remove blocks

	for (int i = 0; i < bodies.size(); i++)
	{

		if (bodies[i].body->position.y > gameHeigth + 500)
		{
			DestroyPhysicsBody(bodies[i].body);
			bodies.erase(bodies.begin() + i);
			i--;
		}

	}
	for (int i = 0; i < bodies.size(); i++)
	{

		bodies[i].body->velocity.x *= 0.8f;
		bodies[i].body->velocity.y *= 0.8f;

	}

	
#pragma endregion



#pragma region draw map

	for (int i = 0; i <= gameHeigth / 100; i++)
	{
		renderer.renderRectangle({ -100, i * 100, 100, 100 }, { 1,1,1,0.3 }, {}, 0, floorTexture);
		renderer.renderRectangle({ gameWith ,i * 100 , 100, 100 }, { 1,1,1,0.3 }, {}, 0, floorTexture);
	}

	for (int i = 0; i < gameWith / 100; i++)
	{
		renderer.renderRectangle({ i * 100, gameHeigth, 100, 100 }, {}, 0, floorTexture);
	}

	renderer.renderRectangle({ -100, -100, 100, 100 }, {}, 0, topTexture);
	renderer.renderRectangle({ gameWith, -100, 100, 100 }, {}, 0, topTexture);


	//renderer.render9Patch2({ 0, gameHeigth, gameWith, 200 }, 5, Colors_White, { 0,0 }, 0, floorTexture, DefaultTextureCoords, { 0,0.4,0.4,0 });
	//renderer.render9Patch2({ -100,0, 100, gameHeigth + 200 }, 5, Colors_White, { 0,0 }, 0, floorTexture, DefaultTextureCoords, { 0,0.4,0.4,0 });
	//renderer.render9Patch2({ gameWith ,0, 100, gameHeigth + 200 }, 5, Colors_White, { 0,0 }, 0, floorTexture, DefaultTextureCoords, { 0,0.4,0.4,0 });
#pragma endregion
	//podea.Draw(Colors_Red, renderer);

	for (auto& cub : bodies)
	{
		cub.Draw(blockTexture, renderer);
	}

	//for (int i = -1; i <= gameWith / 100; i++)
	//	renderer.renderRectangle({ i * 100, gameHeigth - 200, 100, 100 }, {}, 0, topTexture);


#pragma region render strings

	{
		for (int i = 0; i < 2; i++)
		{
			const int elements = 20 * (((float)wireSize[i] - (float)wireSizeMinEnarge) / ((float)wireSizeMaxEnarge - (float)wireSizeMinEnarge)) + 10;

			glm::vec2 vecDir = crane.getPos() - players[i];
			glm::vec2 drawPos = players[i];


			for (int c = 1; c < elements; c++)
			{
				float advance = (float)c / (float)elements;
				glm::vec2 pos = vecDir * advance + drawPos;

				renderer.renderRectangle({ pos.x , pos.y, 10, 10 }, {}, 0, stringTexture);
			}
		}
	}
	crane.Draw(macaraAnim.texture, renderer, macaraAnim.getTexturePos());

#pragma endregion


	renderer.flush();

	return true;
}

void closeGame()
{
	//save files.
	//we don't have to clear resources like textures since os does that for us :P
}