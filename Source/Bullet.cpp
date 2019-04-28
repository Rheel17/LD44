/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#include "Bullet.h"

#include <cmath>
#include <vector>

#include "Wall.h"

Bullet::Bullet(float x, float y, float vx, float vy, const glm::vec4& color) :
		Entity(x, y, 0.0f),
		_start_vx(vx), _start_vy(vy), _color(color) {}

void Bullet::Render(const glm::ivec2& screenDimensions) const {
	_PrepareRenderer();

	_line_shader.Use();
	_line_shader["screenDimensions"] = (glm::vec2) screenDimensions;
	_line_shader["location"] = glm::vec2 { _x, _y };
	_line_shader["color"] = _color;
	_line_shader["rotation"] = _rotation;
	_line_shader["rotationCenter"] = glm::vec2 { 0.5f, 0.5f };

	glBindVertexArray(_vao);
	glDrawArrays(GL_LINE_LOOP, 0, _count);
	glBindVertexArray(0);
}

void Bullet::OnCollisionStart(Entity *other) {
	Die();
}

b2BodyDef Bullet::_CreateBody() const {
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.linearVelocity.x = _start_vx;
	body.linearVelocity.y = _start_vy;
	body.bullet = true;
	body.gravityScale = 0.0f;
	return body;
}

void Bullet::_CreateFixture(std::shared_ptr<b2Body> body) const {
	b2CircleShape shape;
	shape.m_radius = 0.05f;

	b2Fixture *fixture = body->CreateFixture(&shape, 1.0f);
	fixture->SetRestitution(1.0f);
}

void Bullet::_PrepareRenderer() const {
	_PrepareShader();

	if (_is_renderer_prepared) {
		return;
	}

	// prepare the VAO/VBO for rendering
	std::vector<GLfloat> data;

	for (float f = 0; f <= 2 * M_PI; f += 0.2f) {
		data.push_back(0.5f + cos(f) * 0.15f);
		data.push_back(0.5f + sin(f) * 0.15f);
	}

	_count = data.size() / 2;

	_vao = GL::GenVertexArray();
	_vbo = GL::GenBuffer();

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *) 0);

	glBindVertexArray(0);

	_is_renderer_prepared = true;
}

