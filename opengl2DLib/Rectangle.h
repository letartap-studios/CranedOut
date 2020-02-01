#pragma once



#define PHYSAC_STANDALONE
#define PHYSAC_NO_THREADS
#include "physac.h"
#include "opengl2Dlib.h"

struct RectangleBody
{
	PhysicsBody body = 0;
	float width, height;

	RectangleBody() = default;

	RectangleBody(const float xPos, const float yPos, const float width, const float height, const float density)
	{
		Create(xPos, yPos, width, height, density);
	}
	
	void Create(const float xPos, const float yPos, const float width, const float height, const float density)
	{
		this->width = width;
		this->height = height;
		
		body = CreatePhysicsBodyRectangle({ xPos,yPos }, width, height, density);
	}

	void Draw(const gl2d::Texture texture, gl2d::Renderer2D& renderer) const
	{
		if(body)
			renderer.renderRectangle({ body->position.x, body->position.y, width, height }, { 0,0 }, glm::degrees(-body->orient), texture);
	}

	void Draw( const gl2d::Color4f color, gl2d::Renderer2D& renderer) const
	{
		if (body)
			renderer.renderRectangle({ body->position.x, body->position.y, width, height }, color, {0,0}, glm::degrees(-body->orient));
	}
};
