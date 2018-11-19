#pragma once

#include "MainInclude.h"

class LastStep {
public:
	Vector2<> pos;
	Color color;

	LastStep(const Vector2<double>& pos, const Color & color) : pos(pos), color(color) {
	}
	~LastStep() {
	}
};

class SnowDrop {
private:
	const int MIN_LAST_STEPS = 3;
	const int MAX_LAST_STEPS = 75;
	const double MIN_TIME_BETWEEN_LAST_STEPS = 0.001;
	const double MAX_TIME_BETWEEN_LAST_STEPS = 0.09;
	Vector2<> pos;
	Color color;
	Vector2<> vel;
	Vector2<> minVel;
	Vector2<> maxVel;
	double yReset, minX, maxX;
	bool showLastSteps;
	std::vector<LastStep> lastSteps;
	double timeLeftNextStep;
	int maxSteps;
public:
	SnowDrop(const Vector2<> & pos, const Color & color, const Vector2<> & minVel, const Vector2<> & maxVel, double yReset, double minX, double maxX, bool showLastSteps = false);
	~SnowDrop();

	void update(double deltaTime);
	void checkLastStep(float deltaTime);
	double getNewTimeLeftNextStep();
	void addToLastStep();
	void removeLastStep();
	void render(double deltaTime, SDL_Surface* surface);

	void renderLastSteps(double deltaTime, SDL_Surface * surface);

	const Vector2<>& getPos();
	void setPos(const Vector2<>& pos);
	const Color& getColor();
	void setColor(const Color& color);
	const Vector2<>& getVel();
	void setVel(const Vector2<>& vel);
	const double getYReset();
	void setYReset(double yReset);
	const bool isShowLastSteps();
	void setShowLastSteps(bool showLastSteps);
};