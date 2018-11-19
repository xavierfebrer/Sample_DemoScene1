#pragma once

#include "Transition.h"

class SquareFillAnimation : public Transition {
public:
	float delay, initDurationStart, initDuration, initDurationEnd, durationStart, duration, durationEnd;
	std::vector<Color> colors;
	char *dispX, *dispY;
	int rows, columns;

	SquareFillAnimation(SDL_Surface* surface, float delay, float durationStart, float duration, float durationEnd, int rows, int columns);
	virtual ~SquareFillAnimation() override;

	virtual void update(float deltaTime) override;
	virtual void render(float deltaTime) override;
	virtual bool isFinished() override;
	SDL_Surface * loadImage(std::string pathImage);
	void drawImage(SDL_Surface * surface, int x, int y, float alpha);
};
