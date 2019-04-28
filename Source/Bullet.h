/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#ifndef BULLET_H_
#define BULLET_H_

#include "Entity.h"

class Bullet : public Entity {

public:
	Bullet(float x, float y, float vx, float vy, const glm::vec4& color);

	void Render(const glm::ivec2& screenDimensions) const;

	void OnCollisionStart(Entity *other);

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
	glm::vec4 _color;

};

#endif
