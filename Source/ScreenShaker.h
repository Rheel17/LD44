/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#ifndef SCREENSHAKER_H_
#define SCREENSHAKER_H_

#include <glm/glm.hpp>

class ScreenShaker {

public:
	ScreenShaker(const glm::vec2& direction, float shakeFactor, float dampingFactor, float amplitude);

	glm::vec2 Update(float dt);
	float T();

private:
	glm::vec2 _direction;
	float _shake_factor;
	float _damping_factor;
	float _t = 0.0f;


};

#endif
