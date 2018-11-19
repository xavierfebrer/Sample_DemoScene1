#include "..\..\..\include\SquareFillAnimation.h"
#include "../../../include/FinalDemo.h"

SquareFillAnimation::SquareFillAnimation(SDL_Surface* surface,
	float delay, float durationStart, float duration, float durationEnd, int rows, int columns)
	: Transition(surface),
	initDurationStart(durationStart), initDuration(duration), initDurationEnd(durationEnd),
	durationStart(durationStart), duration(duration), durationEnd(durationEnd), delay(delay),
	rows(rows), columns(columns) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			colors.push_back(Color(1, Util::random(), Util::random(), Util::random()));
		}
	}
}

SquareFillAnimation::~SquareFillAnimation() {
}

void SquareFillAnimation::update(float deltaTime) {
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

void SquareFillAnimation::render(float deltaTime) {
	if (delay > 0) return;
	if (durationStart > 0) {
		drawImage(surface, 0, 0, -((durationStart/initDurationStart) - 1));
	} else if (duration > 0) {
		drawImage(surface, 0, 0, 1);
	} else if (durationEnd > 0) {
		drawImage(surface, 0, 0, durationEnd/initDurationEnd);
	}
}

bool SquareFillAnimation::isFinished() {
	return durationStart <= 0 && duration <= 0 && durationEnd <= 0;
}

SDL_Surface* SquareFillAnimation::loadImage(std::string pathImage) {
	SDL_Surface* temp = IMG_Load(pathImage.c_str());
	if (temp == NULL) {
		std::cout << "Image can be loaded! " << IMG_GetError();
		exit(1);
	}
	SDL_Surface* flashTexture = SDL_ConvertSurfaceFormat(temp, SDL_PIXELFORMAT_ARGB8888, 0);
	return flashTexture;
}

void SquareFillAnimation::drawImage(SDL_Surface* surface, int x, int y, float alpha) {
	Uint8 *dst;
	Uint8 *initbuffer = (Uint8 *)surface->pixels;
	int bpp = surface->format->BytesPerPixel;
	double percent = 0;
	int percentCount = 0;
	if(initDurationStart > 0) {
		percent += (durationStart / initDurationStart);
		percentCount++;
	}
	if(initDuration > 0) {
		percent += (duration / initDuration);
		percentCount++;
	}
	if(initDurationEnd > 0) {
		percent += (durationEnd / initDurationEnd);
		percentCount++;
	}
	if(percentCount > 0) {
		percent /= percentCount;
	}
	SDL_LockSurface(surface);
	for (int j = 0; j<FinalDemo::SCREEN_HEIGHT; j++) {
		dst = initbuffer + j *surface->pitch;
		for (int i = 0; i<FinalDemo::SCREEN_WIDTH; i++) {
			int columnIndex = (j / (FinalDemo::SCREEN_HEIGHT / rows));
			int rowIndex = (i / (FinalDemo::SCREEN_WIDTH/columns));
			int colorIndex = columnIndex * (columns - 1) + rowIndex;
			if (percent * rows * columns < colorIndex) {	// create append squares animation
				Color color = colors[colorIndex];
				Uint32 c = 0xFF000000
					+ (((Uint32)(color.getR() * 255)) << 16)
					+ (((Uint32)(color.getG() * 255)) << 8)
					+ ((Uint32)(color.getG() * 255));
				*(Uint32 *)dst = c;
			}
			dst += bpp;
		}
	}
	SDL_UnlockSurface(surface);
}
