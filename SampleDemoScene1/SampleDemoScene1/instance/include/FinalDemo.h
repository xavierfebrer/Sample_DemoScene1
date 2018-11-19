#pragma once

#include "MainInclude.h"
#include "Transition.h"
#include "ImgHSliceTransition.h"
#include "WaveTransition.h"
#include "SquareFillAnimation.h"
#include "SnowDrop.h"

class FinalDemo {
public:
	static const int SCREEN_WIDTH = 640;
	static const int SCREEN_HEIGHT = 480;
	const int MAX_FPS = 60;
	const int MAX_SNOW_DROPS = 200;
#define BEAT_RATE 0.921f
	const int TIME_MS_BETWEEN_ADD_SNOW_DROP = (int)(BEAT_RATE * 1000 / 2);

	int lastTime = 0, currentTime, deltaTimeCount;
	float dTime;
	float minMSBetweenRenderPerFrame = 1 / (MAX_FPS / 1000.0f);
	SDL_Window* window = NULL;
	SDL_Surface* surface = NULL;
	int lastSnowDropAdded = 0;
	double preventStartupSnowfall = BEAT_RATE * 8.0;
	std::vector<SnowDrop*> snowDrops;
	std::vector<Transition*> transitions;
	int bgColor;
	bool timeLeftStage2SnowDropAchieved = false;
	double timeLeftStage2SnowDrop = BEAT_RATE * 88.5 - preventStartupSnowfall;
	bool timeLeftStage3SnowDropAchieved = false;
	double timeLeftStage3SnowDrop = BEAT_RATE * 8;
	bool timeLeftStage4SnowDropAchieved = false;
	double timeLeftStage4SnowDrop = BEAT_RATE * 8;
	bool timeLeftStage5SnowDropAchieved = false;
	double timeLeftStage5SnowDrop = BEAT_RATE * 8;
	bool timeLeftStage6SnowDropAchieved = false;
	double timeLeftStage6SnowDrop = BEAT_RATE * 2;
	bool timeLeftStage7SnowDropAchieved = false;
	double timeLeftStage7SnowDrop = BEAT_RATE * 2;
	bool timeLeftStage8SnowDropAchieved = false;
	double timeLeftStage8SnowDrop = BEAT_RATE * 2;
	bool timeLeftStage9SnowDropAchieved = false;
	double timeLeftStage9SnowDrop = BEAT_RATE * 2;
	bool timeLeftStage10SnowDropAchieved = false;
	double timeLeftStage10SnowDrop = BEAT_RATE * 8;

	Mix_Music *song;
#define BPM_MUSIC 128
#define MSEG_BPM (60000 / BPM_MUSIC)
#define FLASH_MAX_TIME 300
	int flashtime;
	int MusicCurrentTime;
	int MusicCurrentTimeBeat;
	int MusicCurrentBeat;
	int MusicPreviousBeat;

	virtual ~FinalDemo();

	bool init();
	bool initSDL();
	void initMusic();
	void initMainBeatPulse();
	void update(float deltaTime);
	void updateSnowDrops(float deltaTime);
	void updateSnowDropShow(float deltaTime);
	void updateSnowDropEffects(double deltaTime, double & timeLeft, bool & achieved, bool randomColor);
	void updateTransitions(float deltaTime);
	void updateMusic(float deltaTime);
	void render(float deltaTime);
	void renderSnowDrops(float deltaTime);
	void renderTransitions(float deltaTime);
	void waitTime();

	int execute();
};
