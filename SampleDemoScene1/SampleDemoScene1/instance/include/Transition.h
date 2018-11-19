#pragma once

#include "MainInclude.h"

class Transition {
public:
	SDL_Surface* surface;

	Transition(SDL_Surface* surface);
	virtual ~Transition();

	virtual void update(float deltaTime);
	virtual void render(float deltaTime);

	virtual bool isFinished();
};