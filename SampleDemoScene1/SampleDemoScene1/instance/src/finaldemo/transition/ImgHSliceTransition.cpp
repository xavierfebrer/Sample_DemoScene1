#include "..\..\..\include\ImgHSliceTransition.h"
#include "../../../include/FinalDemo.h"

ImgHSliceTransition::ImgHSliceTransition(SDL_Surface* surface, const std::string & pathImage,
	float delay, float durationStart, float duration, float durationEnd,
	Color color)
	: Transition(surface), pathImage(pathImage), image(nullptr),
	initDurationStart(durationStart), initDuration(duration), initDurationEnd(durationEnd),
	durationStart(durationStart), duration(duration), durationEnd(durationEnd), delay(delay),
	color(color) {
	image = loadImage(pathImage);
}

ImgHSliceTransition::~ImgHSliceTransition() {
	SDL_FreeSurface(image);
}

void ImgHSliceTransition::update(float deltaTime) {
	if (delay > 0) {
		delay -= deltaTime;
	} else if (durationStart > 0) {
		durationStart -= deltaTime;
	} else if (duration > 0) {
		duration -= deltaTime;
	} else if (durationEnd > 0) {
		durationEnd -= deltaTime;
	}
}

void ImgHSliceTransition::render(float deltaTime) {
	if (delay > 0) return;
	if (durationStart > 0) {
		drawImage(surface, image, 0, 0, -((durationStart/initDurationStart) - 1));
	} else if (duration > 0) {
		drawImage(surface, image, 0, 0, 1);
	} else if (durationEnd > 0) {
		drawImage(surface, image, 0, 0, durationEnd/initDurationEnd);
	}
}

bool ImgHSliceTransition::isFinished() {
	return durationStart <= 0 && duration <= 0 && durationEnd <= 0;
}

SDL_Surface* ImgHSliceTransition::loadImage(std::string pathImage) {
	SDL_Surface* temp = IMG_Load(pathImage.c_str());
	if (temp == NULL) {
		std::cout << "Image can be loaded! " << IMG_GetError();
		exit(1);
	}
	SDL_Surface* flashTexture = SDL_ConvertSurfaceFormat(temp, SDL_PIXELFORMAT_ARGB8888, 0);
	return flashTexture;
}

void ImgHSliceTransition::drawImage(SDL_Surface* surface, SDL_Surface* img, int x, int y, float alpha) {
	SDL_SetSurfaceAlphaMod(img, (Uint8)((color.getA() * alpha) * 255));
	SDL_BlitSurface(img, NULL, surface, NULL);
	SDL_SetSurfaceAlphaMod(img, 255);
}
