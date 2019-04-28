/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#include "ScreenShaker.h"

#include <cmath>

ScreenShaker::ScreenShaker(const glm::vec2& direction, float shakeFactor, float dampingFactor, float amplitude)
		: _direction(direction * amplitude), _shake_factor(shakeFactor), _damping_factor(dampingFactor) {}

glm::vec2 ScreenShaker::Update(float dt) {
	_t += dt;
	return _direction * expf(-_damping_factor * _t) * sinf(_shake_factor * _t);
}

float ScreenShaker::T() {
	return _t;
}
