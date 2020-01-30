#pragma once
#include "opengl2Dlib.h"

//called once when the game starts
bool initGame(gl2d::Renderer2D &renderer);

//called every frame
bool gameLoop(float deltaTime, gl2d::Renderer2D &renderer, int w, int h);

//called when the game closes
void closeGame();