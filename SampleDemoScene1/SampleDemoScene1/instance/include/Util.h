#pragma once

#include <random>

class Util {
public:
	static double random(double min, double max) {
		double lower_bound = min < max ? min : max;
		double upper_bound = max < min ? min : max;
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<double> dis(lower_bound, upper_bound);
		return dis(gen);
	}

	static double random() {
		return random(0, 1);
	}
};