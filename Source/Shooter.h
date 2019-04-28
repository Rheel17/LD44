/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#ifndef SHOOTER_H_
#define SHOOTER_H_

#include "Wall.h"

class Shooter : public Wall {

public:
	Shooter(unsigned x, unsigned y, float shootTime, float currentTime);
	~Shooter() = default;

	void AddShootingDirection(int dx, int dy);

	void Render(const glm::ivec2& screenDimensions) const;

protected:
	void InternalUpdate(float dt, Level& level);

private:
	void _PrepareRenderer() const;

	std::vector<glm::ivec2> _shooting_directions;
	const float _shoot_time;
	float _current_time = 0.0f;

	mutable GLhandle _vao;
	mutable GLhandle _vbo;
	mutable unsigned _count = 0;
	mutable bool _is_renderer_prepared = false;
};

#endif
