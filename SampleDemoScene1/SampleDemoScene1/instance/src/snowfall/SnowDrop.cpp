#include "../../include/SnowDrop.h"

SnowDrop::SnowDrop(const Vector2<double>& pos, const Color & color,
	const Vector2<> & minVel, const Vector2<> & maxVel,
	double yReset, double minX, double maxX, bool showLastSteps)
	: pos(pos), color(color), minVel(minVel),
	maxVel(maxVel), yReset(yReset),
	minX(minX), maxX(maxX), vel(0, 0), showLastSteps(showLastSteps),
	timeLeftNextStep(getNewTimeLeftNextStep()),
	maxSteps((int)(MIN_LAST_STEPS + (Util::random() * (MAX_LAST_STEPS - MIN_LAST_STEPS)))) {
}

SnowDrop::~SnowDrop() {
}

void SnowDrop::update(double deltaTime) {
	checkLastStep(deltaTime);

	double deltaX = 400;
	double deltaY = 600;
	double deltaVelX =  (Util::random() * deltaX - (deltaX*0.5)) * deltaTime;
	double deltaVelY =  (Util::random() * deltaY - (deltaY*0.5)) * deltaTime;
	vel.x = std::max(minVel.x, std::min(maxVel.x, vel.x + deltaVelX));
	vel.y = std::max(minVel.y, std::min(maxVel.y, vel.y + deltaVelY));
	pos.Set(pos.x + vel.x * deltaTime, pos.y + vel.y * deltaTime);


	if (pos.y > yReset) {
		pos.y = 0;
		vel.x = 0;
		vel.y = minVel.y;
	}
	if (pos.x < 0) {
		pos.x += maxX;
	} else if (pos.x > maxX) {
		pos.x = maxX - pos.x;
	}
}

void SnowDrop::checkLastStep(float deltaTime) {
	timeLeftNextStep -= deltaTime;
	if (timeLeftNextStep <= 0) {
		timeLeftNextStep = getNewTimeLeftNextStep();
		if (!showLastSteps) {
			removeLastStep();
		} else {
			addToLastStep();
		}
	}
}

double SnowDrop::getNewTimeLeftNextStep() {
	return MIN_TIME_BETWEEN_LAST_STEPS + (Util::random() * (MAX_TIME_BETWEEN_LAST_STEPS - MIN_TIME_BETWEEN_LAST_STEPS));
}

void SnowDrop::addToLastStep() {
	lastSteps.push_back(LastStep(pos, color));
	if (lastSteps.size() > maxSteps) {
		lastSteps.erase(lastSteps.begin());
	}
}

void SnowDrop::removeLastStep() {
	lastSteps.push_back(LastStep(pos, color));	// prevents to detach the tail from the main step
	for (int i = 0; i < 4; i++) {
		if (lastSteps.size() > 0) {
			lastSteps.erase(lastSteps.begin());
		}
	}
}

void SnowDrop::render(double deltaTime, SDL_Surface* surface) {
	renderLastSteps(deltaTime, surface);
	if ((pos.x < 0) || (pos.y < 0)) return;
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + ((int) pos.y) * surface->pitch + ((int) pos.x) * bpp;
	p[0] = color.getB() * 0xff;
	p[1] = color.getG() * 0xff;
	p[2] = color.getR() * 0xff;
	p[3] = color.getA() * 0xff;
}

void SnowDrop::renderLastSteps(double deltaTime, SDL_Surface* surface) {
	for (int i = 0; i < lastSteps.size(); i++) {
		if ((lastSteps[i].pos.x < 0) || (lastSteps[i].pos.y < 0)) continue;
		int bpp = surface->format->BytesPerPixel;
		Uint8 *p = (Uint8 *)surface->pixels + ((int) lastSteps[i].pos.y) * surface->pitch + ((int) lastSteps[i].pos.x) * bpp;
		p[0] = color.getB() * 0xff;
		p[1] = color.getG() * 0xff;
		p[2] = color.getR() * 0xff;
		p[3] = color.getA() * 0xff;
	}
}

const Vector2<>& SnowDrop::getPos() {
	return pos;
}

void SnowDrop::setPos(const Vector2<>& pos) {
	this->pos = pos;
}

const Color& SnowDrop::getColor() {
	return color;
}

void SnowDrop::setColor(const Color& color) {
	this->color = color;
}

const Vector2<>& SnowDrop::getVel() {
	return vel;
}

void SnowDrop::setVel(const Vector2<>& vel) {
	this->vel = vel;
}

const double SnowDrop::getYReset() {
	return yReset;
}

void SnowDrop::setYReset(double yReset) {
	this->yReset = yReset;
}

const bool SnowDrop::isShowLastSteps() {
	return false;
}

void SnowDrop::setShowLastSteps(bool showLastSteps) {
	this->showLastSteps = showLastSteps;
}
