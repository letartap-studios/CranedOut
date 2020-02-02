#pragma once
#include "opengl2Dlib.h"

struct Animate
{

	Animate() = default;
	Animate(int w, int h, int frameTime, gl2d::Texture t) { create(w, h, frameTime, t); };

	void create(int w, int h, int frameTime, gl2d::Texture t)
	{
		this->w = w;
		this->h = h;
		this->frameTime = frameTime;
		this->texture = t;
	}

	int posX = 0;
	int posY = 0;

	int w;
	int h;
	float frameTime;

	gl2d::Texture texture;

	void updateTime(float deltaTime);
	glm::vec4 getTexturePos();

private:
	float timePassed = 0;
};

