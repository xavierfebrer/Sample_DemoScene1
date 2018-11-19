#include "..\..\..\include\Transition.h"

Transition::Transition(SDL_Surface* surface) : surface(surface) {
}

Transition::~Transition() {
}

void Transition::update(float deltaTime) {
}

void Transition::render(float deltaTime) {
}

bool Transition::isFinished() {
	return true;
}
