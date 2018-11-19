#pragma once

#include "MainInclude.h"

class Color {
private:
	double a, r, g, b;	// [0, 1]
public:
	const double MIN_VALUE = 0;
	const double MAX_VALUE = 1;

	Color(double a, double r, double g, double b) {
		setA(a);
		setR(r);
		setG(g);
		setB(b);
	}

	Color(const Color & color) {
		setA(color.a);
		setR(color.r);
		setG(color.g);
		setB(color.b);
	}

	inline Color & operator=(const Color & c) { a = c.a; r = c.r; g = c.g; b = c.b; return *this; }

	inline Color & operator=(double c) { a = r = g = b = c; return *this; }

	double getA() {
		return a;
	}

	void setA(double a) {
		this->a = std::min(std::max(a, MIN_VALUE), MAX_VALUE);
	}

	double getR() {
		return r;
	}

	void setR(double r) {
		this->r = std::min(std::max(r, MIN_VALUE), MAX_VALUE);
	}

	double getG() {
		return g;
	}

	void setG(double g) {
		this->g = std::min(std::max(g, MIN_VALUE), MAX_VALUE);
	}

	double getB() {
		return b;
	}

	void setB(double b) {
		this->b = std::min(std::max(b, MIN_VALUE), MAX_VALUE);
	}
};