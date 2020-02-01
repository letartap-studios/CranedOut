#include "Animate.h"

void Animate::updateTime(float deltaTime)
{
	timePassed += deltaTime;
	if(timePassed > frameTime)
	{
		timePassed = frameTime - timePassed;
		posX++;
		if(posX >= w)
		{
			posX = 0;
		}
	}
}

glm::vec4 Animate::getTexturePos()
{
	glm::vec2 tSize = texture.GetSize();
	glm::vec2 scale = glm::vec2{ 1,1 } / glm::vec2{w,h};


	glm::vec4 tCoord;
	tCoord.x = scale.x * posX;
	tCoord.y = scale.y * posY;
	tCoord.z = scale.x * (posX + 1);
	tCoord.w = scale.y * (posY + 1);

	return tCoord;
}
