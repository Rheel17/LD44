/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#include <Box2D/Box2D.h>
#include <glm/glm.hpp>

#include "ShaderProgram.h"

class Level;

class Entity {

public:
	Entity(float x, float y, float rotation);
	virtual ~Entity() = default;

	void Initialize(std::shared_ptr<b2World> world);

	void Update(float dt, Level& level);
	virtual void Render(const glm::ivec2& screenDimensions) const = 0;

	virtual void OnCollisionStart(Entity *other) {}
	virtual void OnCollisionEnd(Entity *other) {}

	float X();
	float Y();

	void Die();
	bool IsAlive();

protected:
	float _x;
	float _y;
	float _rotation;
	float _alive = true;

	std::shared_ptr<b2Body> _b2_body;

	virtual void InternalUpdate(float dt, Level& level) {}
	virtual b2BodyDef _CreateBody() const = 0;
	virtual void _CreateFixture(std::shared_ptr<b2Body> body) const = 0;

protected:
	static ShaderProgram _line_shader;
	static bool _is_shader_prepared;
	static void _PrepareShader();
};

#endif
