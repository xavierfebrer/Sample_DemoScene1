#include "../../include/FinalDemo.h"

int FinalDemo::execute() {
	if (!init()) return -1;

	bool quit = false;
	SDL_Event e;
	while( !quit ) {
		while( SDL_PollEvent( &e ) != 0 ) {
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
					quit = true;
				}
			}
			if (e.type == SDL_QUIT) {
				quit = true;
			}
		}
		update(dTime);
		render(dTime);
		SDL_UpdateWindowSurface(window);
		waitTime();
	}
	return 0;
}

FinalDemo::~FinalDemo() {
	for (int i = transitions.size()-1; i >= 0; --i) {
		delete transitions[i];
		transitions.erase(transitions.begin() + i);
	}
	for (int i = snowDrops.size()-1; i >= 0; --i) {
		delete snowDrops[i];
		snowDrops.erase(snowDrops.begin() + i);
	}
	Mix_HaltMusic();
	Mix_FreeMusic(song);
	Mix_Quit();
	Mix_CloseAudio();
	SDL_DestroyWindow(window);
	SDL_Quit();
}

bool FinalDemo::init() {
	if (!initSDL()) {
		return false;
	}
	int init = IMG_Init(IMG_INIT_PNG);
	if(init == 0) {
		auto error = IMG_GetError();
		printf("IMG_Init: Failed to init required jpg and png support!\n");
		printf("IMG_Init: %s\n", error);
		return false;
	}
	initMusic();
	initMainBeatPulse();
	bgColor = 0xFF000000;
	return true;
}

bool FinalDemo::initSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return false;
	}
	window = SDL_CreateWindow("Sample DemoScene 1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		return false;
	}
	surface = SDL_GetWindowSurface(window);
	if (surface == NULL) {
		return false;
	}
	return true;
}

void FinalDemo::initMusic() {
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	Mix_Init(MIX_INIT_OGG);
	/*
	Song Length: 3:26
	Song Status: Active 
	Song Bitrate: 199 kbps 
	Song Legacy Flag: R 
	Song Source: Oldskool 
	*/
	song =  Mix_LoadMUS("assets/rhino-chipolution.ogg");
	if (!song) {
		std::cout << "Error loading Music: " << Mix_GetError() << std::endl;
		exit(1);
	}
	Mix_PlayMusic(song,0);
	flashtime = 0;
	MusicCurrentTime = 0;
	MusicCurrentTimeBeat = 0;
	MusicCurrentBeat = 0;
	MusicPreviousBeat = -1;
}

void FinalDemo::initMainBeatPulse() {
	float count = 1;	// counter beat
	float drop = 25.0f;

	double initSquareduration = 4.25 + 4;
	transitions.push_back(new SquareFillAnimation(surface, BEAT_RATE * 0,
		BEAT_RATE * initSquareduration/3,
		BEAT_RATE * initSquareduration/3,
		BEAT_RATE * initSquareduration/3, 8, 8));
	transitions.push_back(new SquareFillAnimation(surface, BEAT_RATE * 151.0f,
		BEAT_RATE * 2,
		BEAT_RATE * 2,
		BEAT_RATE * 2, 4, 4));
	count -= 0.125;	// fix delay of start fade animation
	for (int i = 0; i < drop; i++) {
		transitions.push_back(new ImgHSliceTransition(surface, "assets/logo1.png", BEAT_RATE * count + BEAT_RATE * 4,	// x4 -> after square delay logo
			BEAT_RATE * ( (1- ((drop-i)/drop)) /6.0f),
			BEAT_RATE * ( (1- ((drop-i)/drop)) /2.0f),
			BEAT_RATE * ( (1- ((drop-i)/drop)) /1.0f), Color(0.75*i/drop, 1, 0, 0)));
		if (i == 7) {
			transitions.push_back(new ImgHSliceTransition(surface, "assets/logo1.png", BEAT_RATE * count + BEAT_RATE,
				BEAT_RATE * ( (1- ((drop-i)/drop)) /6.0f),
				BEAT_RATE * ( (1- ((drop-i)/drop)) /2.0f),
				BEAT_RATE * ( (1- ((drop-i)/drop)) /1.0f), Color(0.75*i/drop, 1, 0, 0)));
		} else if (i == 14) {
			transitions.push_back(new ImgHSliceTransition(surface, "assets/logo1.png", BEAT_RATE * count + BEAT_RATE,
				BEAT_RATE * ( (1- ((drop-i)/drop)) /6.0f),
				BEAT_RATE * ( (1- ((drop-i)/drop)) /2.0f),
				BEAT_RATE * ( (1- ((drop-i)/drop)) /1.0f), Color(0.75*i/drop, 1, 0, 0)));
		} else if (i == 19) {
			transitions.push_back(new ImgHSliceTransition(surface, "assets/logo1.png", BEAT_RATE * count + BEAT_RATE,
				BEAT_RATE * ( (1- ((drop-i)/drop)) /6.0f),
				BEAT_RATE * ( (1- ((drop-i)/drop)) /2.0f),
				BEAT_RATE * ( (1- ((drop-i)/drop)) /1.0f), Color(0.75*i/drop, 1, 0, 0)));
		}
		count += 2;
	}
	count += 0.125;
	count += 4;
	transitions.push_back(new ImgHSliceTransition(surface, "assets/logo2.png", BEAT_RATE * count,
		BEAT_RATE * 3,
		BEAT_RATE * 1,
		BEAT_RATE * 15, Color(1, 0, 0, 1)));
	count += 4;
	transitions.push_back(new WaveTransition(surface, "assets/logo2.png", BEAT_RATE * count,
		BEAT_RATE * 2,
		BEAT_RATE * 15,
		BEAT_RATE * 12.5f, 1));
}

void FinalDemo::update(float deltaTime){
	updateSnowDrops(deltaTime);
	updateTransitions(deltaTime);
	updateMusic(deltaTime);
}

void FinalDemo::updateSnowDrops(float deltaTime) {
	if (snowDrops.size() < MAX_SNOW_DROPS && SDL_GetTicks() - lastSnowDropAdded > TIME_MS_BETWEEN_ADD_SNOW_DROP) {
		if (preventStartupSnowfall > 0) {
			preventStartupSnowfall -= deltaTime;
			return;
		}
		Vector2<> pos(Util::random() * SCREEN_WIDTH, 0);
		Vector2<> minVel(-50, 50);
		Vector2<> maxVel(50, 500);
		bool showLastSteps = Util::random() < 0.01;
		Color* color;
		if (!timeLeftStage2SnowDropAchieved) {
			snowDrops.push_back(new SnowDrop(pos, Color(1, 1, 1, 0), minVel, maxVel, SCREEN_HEIGHT, 0, SCREEN_WIDTH, false));
		} else {
			snowDrops.push_back(new SnowDrop(pos, Color(0.2 + Util::random() * 0.8,
				0.1 + Util::random() * 0.9,
				0.1 + Util::random() * 0.9,
				0.1 + Util::random() * 0.9), minVel, maxVel, SCREEN_HEIGHT, 0, SCREEN_WIDTH, showLastSteps));
		}
		lastSnowDropAdded = SDL_GetTicks();
	}
	for (std::vector<SnowDrop*>::iterator it = snowDrops.begin(); it != snowDrops.end(); ++it) {
		(*(*it)).update(deltaTime);
	}
	updateSnowDropShow(deltaTime);
}

void FinalDemo::updateSnowDropShow(float deltaTime){
	if (!timeLeftStage2SnowDropAchieved) {
		timeLeftStage2SnowDrop -= deltaTime;
		if (timeLeftStage2SnowDrop <= 0) {
			timeLeftStage2SnowDropAchieved = true;
			for (std::vector<SnowDrop*>::iterator it = snowDrops.begin(); it != snowDrops.end(); ++it) {
				(*(*it)).setShowLastSteps(Util::random() < 0.75);
				Color color(0.2 + Util::random() * 0.8,
					0.1 + Util::random() * 0.9,
					0.1 + Util::random() * 0.9,
					0.1 + Util::random() * 0.9);	// minimum of 0.2 alpha to see something, minimum 0.1 of each color to not see black over black background
				(*(*it)).setColor(color);
			}
		}
	} else if (!timeLeftStage3SnowDropAchieved) {
		updateSnowDropEffects(deltaTime, timeLeftStage3SnowDrop, timeLeftStage3SnowDropAchieved, false);
	} else if (!timeLeftStage4SnowDropAchieved) {
		updateSnowDropEffects(deltaTime, timeLeftStage4SnowDrop, timeLeftStage4SnowDropAchieved, true);
	} else if (!timeLeftStage5SnowDropAchieved) {
		updateSnowDropEffects(deltaTime, timeLeftStage5SnowDrop, timeLeftStage5SnowDropAchieved, false);
	} else if (!timeLeftStage6SnowDropAchieved) {
		updateSnowDropEffects(deltaTime, timeLeftStage6SnowDrop, timeLeftStage6SnowDropAchieved, true);
	} else if (!timeLeftStage7SnowDropAchieved) {
		updateSnowDropEffects(deltaTime, timeLeftStage7SnowDrop, timeLeftStage7SnowDropAchieved, false);
	} else if (!timeLeftStage8SnowDropAchieved) {
		updateSnowDropEffects(deltaTime, timeLeftStage8SnowDrop, timeLeftStage8SnowDropAchieved, true);
	} else if (!timeLeftStage9SnowDropAchieved) {
		updateSnowDropEffects(deltaTime, timeLeftStage9SnowDrop, timeLeftStage9SnowDropAchieved, false);
	} else if (!timeLeftStage10SnowDropAchieved) {
		updateSnowDropEffects(deltaTime, timeLeftStage10SnowDrop, timeLeftStage10SnowDropAchieved, true);
	}
}

void FinalDemo::updateSnowDropEffects(double deltaTime, double & timeLeft, bool & achieved, bool randomColor) {
	timeLeft -= deltaTime;
	if (timeLeft <= 0) {
		achieved = true;
		for (std::vector<SnowDrop*>::iterator it = snowDrops.begin(); it != snowDrops.end(); ++it) {
			(*(*it)).setShowLastSteps(randomColor ? Util::random() < 0.85 : Util::random() < 0.005);
		}
	}
}

void FinalDemo::updateTransitions(float deltaTime) {
	for (int i = transitions.size()-1; i >= 0; --i) {
		transitions[i]->update(deltaTime);
		if (transitions[i]->isFinished()) {
			delete transitions[i];
			transitions.erase(transitions.begin() + i);
		}
	}
}

void FinalDemo::updateMusic(float deltaTime) {
	MusicCurrentTime += deltaTimeCount;
	MusicCurrentTimeBeat += deltaTimeCount;
	MusicPreviousBeat = MusicCurrentBeat;
	if (MusicCurrentTimeBeat >= MSEG_BPM) {
		MusicCurrentTimeBeat = 0;
		MusicCurrentBeat ++;
		flashtime = FLASH_MAX_TIME;
	}
	if (flashtime > 0) {
		flashtime -= deltaTimeCount;
	}
	else {
		flashtime = 0;
	}
	if (!Mix_PlayingMusic()) {
		exit(0);
	}
}

void FinalDemo::render(float deltaTime) {
	SDL_FillRect(surface, NULL, bgColor);
	renderSnowDrops(deltaTime);
	renderTransitions(deltaTime);
}

void FinalDemo::renderSnowDrops(float deltaTime) {
	for (int i = snowDrops.size()-1; i >= 0; --i) {
		snowDrops[i]->render(deltaTime, surface);
	}
}

void FinalDemo::renderTransitions(float deltaTime) {
	for (int i = transitions.size()-1; i >= 0; --i) {
		transitions[i]->render(deltaTime);
	}
}

void FinalDemo::waitTime() {
	currentTime = SDL_GetTicks();
	deltaTimeCount = currentTime - lastTime;
	dTime = (currentTime - lastTime) / 1000.0f;
	if (deltaTimeCount < (int)minMSBetweenRenderPerFrame) {
		SDL_Delay((int)minMSBetweenRenderPerFrame - deltaTimeCount);
	}
	lastTime = currentTime;
}