#pragma once

#include "MainInclude.h"
#include "SnowDrop.h"

class SnowDemo {
private:
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	const int MAX_SNOW_DROPS = 1000;
	const int TIME_MS_BETWEEN_ADD_SNOW_DROP = 50;
	const int MAX_FPS = 60;

	int lastTime = 0, currentTime, deltaTimeCount;
	float dTime;
	float minMSBetweenRenderPerFrame = 1 / (MAX_FPS / 1000.0f);
	SDL_Window* window = NULL;
	SDL_Surface* surface = NULL;
	bool gRenderQuad = true;
	int lastSnowDropAdded = 0;
	std::vector<SnowDrop> snowDrops;

	bool init();
	bool initSDL();
	bool initGL();
	void initSnowDrops();
	void update(float deltaTime);
	void updateSnowDrops(float deltaTime);
	void render(float deltaTime);
	void renderBG(float deltaTime);
	void renderSnowDrops(float deltaTime);
	void waitTime();
	void close();
public:
	int execute();
};