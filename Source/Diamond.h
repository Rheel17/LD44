/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#ifndef DIAMOND_H_
#define DIAMOND_H_

#include "Entity.h"

class Diamond : public Entity {

public:
	Diamond(float x, float y, float vx, float vy);

	void Render(const glm::ivec2& screenDimensions, const glm::vec3& cameraParams) const;

protected:
	b2BodyDef _CreateBody() const;
	void _CreateFixture(std::shared_ptr<b2Body> body) const;

private:
	void _PrepareRenderer() const;

	mutable GLhandle _vao;
	mutable GLhandle _vbo;
	mutable unsigned _count = 0;
	mutable bool _is_renderer_prepared = false;

	float _start_vx;
	float _start_vy;

};

#endif
