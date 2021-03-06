/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <set>
#include <functional>

#include "Bullet.h"
#include "Wall.h"

class Player : public Entity {
	friend class PlayerController;

public:
	Player(float x, float y);
	~Player() = default;

	void Render(const glm::ivec2& screenDimensions, const glm::vec3& cameraParams) const;

	void OnCollisionStart(Entity *other, b2Contact *contact);
	void OnCollisionEnd(Entity *other, b2Contact *contact);
	bool IsGrouded() const;

	unsigned Score() const;
	unsigned Health() const;

protected:
	b2BodyDef _CreateBody() const;
	void _CreateFixture(std::shared_ptr<b2Body> body) const;

public:
	static constexpr unsigned max_hp = 20;

private:
	void _PrepareRenderer() const;

	mutable GLhandle _vao;
	mutable GLhandle _vbo;
	mutable unsigned _count = 0;
	mutable bool _is_renderer_prepared = false;

	std::set<Wall *> _touching_walls;
	std::shared_ptr<Bullet> _display_bullet;
	unsigned _bullet_count = 0;

	unsigned _score = 0;

	unsigned _hp = max_hp;

};

#endif
