#include "..\..\..\include\WaveTransition.h"
#include "../../../include/FinalDemo.h"

WaveTransition::WaveTransition(SDL_Surface* surface, const std::string & pathImage,
	float delay, float durationStart, float duration, float durationEnd,
	float alpha)
	: Transition(surface), pathImage(pathImage), image(nullptr),
	initDurationStart(durationStart), initDuration(duration), initDurationEnd(durationEnd),
	durationStart(durationStart), duration(duration), durationEnd(durationEnd), delay(delay),
	alpha(alpha), currentDeleteRow(0), timeUpdateDeletion(TIME_UPDATE_DELETION) {
	image = loadImage(pathImage);
	initWave();
}

WaveTransition::~WaveTransition() {
	SDL_FreeSurface(image);
}

void WaveTransition::initWave() {
	dispX = (char*)malloc(FinalDemo::SCREEN_WIDTH * FinalDemo::SCREEN_HEIGHT * 4);
	dispY = (char*)malloc(FinalDemo::SCREEN_WIDTH * FinalDemo::SCREEN_HEIGHT * 4);
	preCalc();
}

void WaveTransition::preCalc() {
	int i, j, dst;
	dst = 0;
	for (j = 0; j<(FinalDemo::SCREEN_HEIGHT * 2); j++) {
		for (i = 0; i<(FinalDemo::SCREEN_WIDTH * 2); i++) {
			float x = (float)i;
			float y = (float)j;
			dispX[dst] = (signed char)(8 * (16 * sin(x / 50)));	// custom selected math function
			dispY[dst] = (signed char)(8 * (16 * cos(x / 50)));	// custom selected math function
			dst++;
		}
	}
}

void WaveTransition::update(float deltaTime) {
	currentTime += deltaTime;
	if (delay > 0) {
		delay -= deltaTime;
	} else {
		if (durationStart > 0) {
			durationStart -= deltaTime;
		} else if (duration > 0) {
			duration -= deltaTime;
		} else if (durationEnd > 0) {
			durationEnd -= deltaTime;
		}
		windowx1 = (FinalDemo::SCREEN_WIDTH / 2) + (int)(((FinalDemo::SCREEN_WIDTH / 2)-1) * cos((double)(currentTime*1000.0) / 2050));
		windowx2 = (FinalDemo::SCREEN_WIDTH / 2) + (int)(((FinalDemo::SCREEN_WIDTH / 2) - 1) * sin((double)-(currentTime*1000.0) / 1970));
		windowy1 = (FinalDemo::SCREEN_HEIGHT / 2) + (int)(((FinalDemo::SCREEN_HEIGHT / 2) - 1) * sin((double)(currentTime*1000.0) / 2310));
		windowy2 = (FinalDemo::SCREEN_HEIGHT / 2) + (int)(((FinalDemo::SCREEN_HEIGHT / 2) - 1) * cos((double)-(currentTime*1000.0) / 2240));

		timeUpdateDeletion -= deltaTime;
		if(timeUpdateDeletion <= 0) {
			timeUpdateDeletion = TIME_UPDATE_DELETION;
			if (currentDeleteRow < FinalDemo::SCREEN_HEIGHT) {
				Uint8 *imagebuffer = (Uint8 *)image->pixels;
				int bppImage = image->format->BytesPerPixel;
				SDL_LockSurface(image);
				for (int i = 0; i < FinalDemo::SCREEN_WIDTH; i++) {
					Uint8 *p = (Uint8 *)imagebuffer + currentDeleteRow * image->pitch
						+ i * bppImage;
					*(Uint32 *)p = 0;
				}
				SDL_UnlockSurface(image);
				currentDeleteRow += 5;
			} else {
				if (currentDeleteRow % 5 == 0) {
					currentDeleteRow = 4;
				} else  if (currentDeleteRow % 4 == 0) {
					currentDeleteRow = 3;
				} else  if (currentDeleteRow % 3 == 0) {
					currentDeleteRow = 2;
				}  else  if (currentDeleteRow % 2 == 0) {
					currentDeleteRow = 1;
				} else {
					currentDeleteRow = 0;
				}
			}
		}
	}
}

void WaveTransition::render(float deltaTime) {
	if (delay > 0) return;
	if (durationStart > 0) {
		drawImage(deltaTime, surface, image, 0, 0, -((durationStart/initDurationStart) - 1));
	} else if (duration > 0) {
		drawImage(deltaTime, surface, image, 0, 0, 1);
	} else if (durationEnd > 0) {
		drawImage(deltaTime, surface, image, 0, 0, durationEnd/initDurationEnd);
	}
}

bool WaveTransition::isFinished() {
	return durationStart <= 0 && duration <= 0 && durationEnd <= 0;
}

SDL_Surface* WaveTransition::loadImage(std::string pathImage) {
	SDL_Surface* temp = IMG_Load(pathImage.c_str());
	if (temp == NULL) {
		std::cout << "Image can be loaded! " << IMG_GetError();
		exit(1);
	}
	SDL_Surface* flashTexture = SDL_ConvertSurfaceFormat(temp, SDL_PIXELFORMAT_ARGB8888, 0);
	return flashTexture;
}

void WaveTransition::drawImage(float deltaTime, SDL_Surface* surface, SDL_Surface* img, int x, int y, float alpha) {
	Uint8 *dst;
	int	src1 = windowy1 * (FinalDemo::SCREEN_WIDTH * 2) + windowx1,
		src2 = windowy2 * (FinalDemo::SCREEN_WIDTH * 2) + windowx2;
	int dX, dY;
	Uint8 *initbuffer = (Uint8 *)surface->pixels;
	int bpp = surface->format->BytesPerPixel;
	Uint8 *imagebuffer = (Uint8 *)image->pixels;
	int bppImage = image->format->BytesPerPixel;

	SDL_LockSurface(surface);
	for (int j = 0; j<FinalDemo::SCREEN_HEIGHT; j++) {
		dst = initbuffer + j *surface->pitch;
		for (int i = 0; i<FinalDemo::SCREEN_WIDTH; i++) {
			dY = j + (dispY[src1] >> 3);
			dX = i + (dispX[src2] >> 3);
			if ((dY >= 0) && (dY<(FinalDemo::SCREEN_HEIGHT - 1)) && (dX >= 0) && (dX<(FinalDemo::SCREEN_WIDTH - 1))) {
				Uint8 *p = (Uint8 *)imagebuffer + dY * image->pitch + dX * bppImage;
				unsigned int Color = (((Uint32)(alpha * 255)) << 24) + (*(Uint32 *)p << 16) + (*(Uint32 *)p << 8) + *(Uint32 *)p;
				if (*(Uint32 *)dst != 0) {
					*(Uint32 *)dst = *(Uint32 *)p;
				}
			}
			dst += bpp; 
			src1++; src2++;
		}
		src1 += FinalDemo::SCREEN_WIDTH;
		src2 += FinalDemo::SCREEN_WIDTH;
	}
	SDL_UnlockSurface(surface);
}
