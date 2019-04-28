/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <set>

#include "Wall.h"

class Player : public Entity {
	friend class PlayerController;

public:
	Player(float x, float y);
	~Player() = default;

	void Render(const glm::ivec2& screenDimensions) const;

	void OnCollisionStart(Entity *other);
	void OnCollisionEnd(Entity *other);
	bool IsGrouded() const;

protected:
	b2BodyDef _CreateBody() const;
	void _CreateFixture(std::shared_ptr<b2Body> body) const;

private:
	void _PrepareRenderer() const;

	mutable GLhandle _vao;
	mutable GLhandle _vbo;
	mutable unsigned _count = 0;
	mutable bool _is_renderer_prepared = false;

	std::set<Wall *> _touching_walls;
	unsigned _bullets = 0;

};

#endif
