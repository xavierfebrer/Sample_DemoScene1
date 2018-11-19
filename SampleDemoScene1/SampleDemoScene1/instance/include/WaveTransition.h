#pragma once

#include "Transition.h"

class WaveTransition : public Transition {
public:
	const float TIME_UPDATE_DELETION = 1/21.0f;
	std::string pathImage;
	SDL_Surface *image;
	float delay, initDurationStart, initDuration, initDurationEnd, durationStart, duration, durationEnd, alpha;
	char *dispX, *dispY;
	int windowx1, windowy1, windowx2, windowy2;
	double currentTime;
	int currentDeleteRow;
	float timeUpdateDeletion;

	WaveTransition(SDL_Surface* surface, const std::string & pathImage, float delay, float durationStart, float duration, float durationEnd, float alpha);
	virtual ~WaveTransition() override;

	void initWave();

	void preCalc();

	virtual void update(float deltaTime) override;
	virtual void render(float deltaTime) override;
	virtual bool isFinished() override;
	SDL_Surface * loadImage(std::string pathImage);
	void drawImage(float deltaTime, SDL_Surface * surface, SDL_Surface * img, int x, int y, float alpha);
};
