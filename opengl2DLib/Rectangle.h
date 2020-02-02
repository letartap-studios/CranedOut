#pragma once



#define PHYSAC_STANDALONE
#define PHYSAC_NO_THREADS
#include "physac.h"
#include "opengl2Dlib.h"

struct RectangleBody
{
	PhysicsBody body = 0;
	float width, height;


	float yPadding = 0;
	
	RectangleBody() = default;

	RectangleBody(const float xPos, const float yPos, const float width, const float height, const float density, const int padd = 0)
	{
		Create(xPos, yPos, width, height, density, padd);
		
	}

	glm::vec2 getPos() const
	{
		return { body->position.x, body->position.y - yPadding };
	}
	
	void setPos(const glm::vec2 pos)
	{
		body->position =  { pos.x, pos.y + yPadding };
	}

	void Create(const float xPos, const float yPos, const float width, const float height, const float density, const int padd = 0)
	{
		yPadding = padd;
		this->width = width;
		this->height = height;
		
		body = CreatePhysicsBodyRectangle({ xPos,yPos + padd}, width, height, density);
		body->dynamicFriction = 0.6;
		body->staticFriction = 0.6;
		
		//body->inertia *= .001 * deltaTime;
	}

	void Draw(const gl2d::Texture texture, gl2d::Renderer2D& renderer, glm::vec4 texCoord = DefaultTextureCoords) const
	{
		if(body)
			renderer.renderRectangle({ body->position.x - width / 2, getPos().y - height / 2, width, height }, { 0,0 }, glm::degrees(-body->orient), texture, texCoord);
	}

	void Draw(const gl2d::Color4f color, gl2d::Renderer2D& renderer) const
	{
		if (body)
			renderer.renderRectangle({ body->position.x - width/2, getPos().y - height/2, width, height }, color, { 0,0 }, glm::degrees(-body->orient));
	}

	float sign(const glm::vec2 p1, const glm::vec2 p2, const glm::vec2 p3)
	{
		return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
	}

	bool PointInTriangle(const glm::vec2 pt, const glm::vec2 v1, const glm::vec2 v2, const glm::vec2 v3)
	{
		bool b1, b2, b3;

		b1 = sign(pt, v1, v2) < 0.0f;
		b2 = sign(pt, v2, v3) < 0.0f;
		b3 = sign(pt, v3, v1) < 0.0f;

		return ((b1 == b2) && (b2 == b3));
	}
	
	bool PointCollision(const glm::vec2 pos)
	{
		glm::vec2 colt[4];

		auto vect = [](const glm::vec2 a, const glm::vec2 b)
		{
			return glm::vec2(b - a);
		};
		
		for (auto i = 0; i < body->shape.vertexData.vertexCount; i++)
		{
			colt[i].x = body->shape.vertexData.positions[i].x + body->position.x;
			colt[i].y = body->shape.vertexData.positions[i].y + body->position.y;
		}


		return PointInTriangle(pos ,colt[0], colt[1], colt[2]) || PointInTriangle(pos, colt[0], colt[3], colt[2]);
	}
};
