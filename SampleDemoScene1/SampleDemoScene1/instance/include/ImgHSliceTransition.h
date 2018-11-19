#pragma once

#include "Transition.h"

class ImgHSliceTransition : public Transition {
public:
	std::string pathImage;
	SDL_Surface *image;
	float delay, initDurationStart, initDuration, initDurationEnd, durationStart, duration, durationEnd;
	Color color;

	ImgHSliceTransition(SDL_Surface* surface, const std::string & pathImage,
		float delay, float durationStart, float duration, float durationEnd, Color color);
	virtual ~ImgHSliceTransition() override;

	virtual void update(float deltaTime) override;
	virtual void render(float deltaTime) override;
	virtual bool isFinished() override;
	SDL_Surface * loadImage(std::string pathImage);
	void drawImage(SDL_Surface * surface, SDL_Surface * img, int x, int y, float alpha);
};
