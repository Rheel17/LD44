/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#include "Entity.h"

#include <iostream>

ShaderProgram Entity::_line_shader;
bool Entity::_is_shader_prepared(false);

Entity::Entity(float x, float y, float rotation) :
		_x(x), _y(y), _rotation(rotation) {}

void Entity::Initialize(std::shared_ptr<b2World> world) {
	// create the body
	b2BodyDef bodyDef = _CreateBody();
	bodyDef.position.x = _x;
	bodyDef.position.y = _y;
	bodyDef.angle = _rotation;
	bodyDef.userData = this;

	_b2_body = std::shared_ptr<b2Body>(world->CreateBody(&bodyDef), [world](b2Body *body) {
		world->DestroyBody(body);
	});

	// create the fixture
	_CreateFixture(_b2_body);
}

void Entity::Update(float dt, Level& level) {
	_x = _b2_body->GetPosition().x;
	_y = _b2_body->GetPosition().y;
	_rotation = _b2_body->GetAngle();

	InternalUpdate(dt, level);
}

float Entity::X() {
	return _x;
}

float Entity::Y() {
	return _y;
}

void Entity::Die() {
	_alive = false;
}

bool Entity::IsAlive() {
	return _alive;
}

void Entity::_PrepareShader() {
	if (_is_shader_prepared) {
		return;
	}

	std::clog << "_PrepareShader()" << std::endl;

	_line_shader.AddShaderFromFile(GL_VERTEX_SHADER, "Resources/shader.vert.glsl");
	_line_shader.AddShaderFromFile(GL_FRAGMENT_SHADER, "Resources/shader.frag.glsl");
	_line_shader.Link();

	_is_shader_prepared = true;
}
