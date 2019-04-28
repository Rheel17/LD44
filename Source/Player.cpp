/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#include "Player.h"

#include "Bullet.h"

Player::Player(float x, float y) :
		Entity(x, y, 0.0f) {}

void Player::Render(const glm::ivec2& screenDimensions) const {
	_PrepareRenderer();

	_line_shader.Use();
	_line_shader["screenDimensions"] = (glm::vec2) screenDimensions;
	_line_shader["location"] = glm::vec2 { _x, _y };
	_line_shader["color"] = glm::vec4 { 1.0f, 0.0f, 1.0f, 1.0f };
	_line_shader["rotation"] = _rotation;
	_line_shader["rotationCenter"] = glm::vec2 { 0.50f, 0.43f };

	glBindVertexArray(_vao);
	glDrawArrays(GL_LINE_STRIP, 0, _count);
	glBindVertexArray(0);
}

void Player::OnCollisionStart(Entity *other) {
	Wall *wall = dynamic_cast<Wall *>(other);
	Bullet *bullet = dynamic_cast<Bullet *>(other);

	if (wall) {
		_touching_walls.insert(wall);
	} else if (bullet) {
		_bullets++;
	}
}

void Player::OnCollisionEnd(Entity *other) {
	Wall *wall = dynamic_cast<Wall *>(other);

	if (wall) {
		_touching_walls.erase(wall);
	}
}

bool Player::IsGrouded() const {
	for (Wall *wall : _touching_walls) {
		if (_y <= wall->Y() + 0.42f &&
				_x - 0.35f <= wall->X() && wall->X() <= _x + 0.35f) {
			return true;
		}
	}

	return false;
}

b2BodyDef Player::_CreateBody() const {
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.angularDamping = 0.98f;
	return bodyDef;
}

void Player::_CreateFixture(std::shared_ptr<b2Body> body) const {
	b2Vec2 shape_data[] = {
		{ -0.25f, -0.43f },
		{  0.25f, -0.43f },
		{  0.50f,  0.00f },
		{  0.25f,  0.43f },
		{ -0.25f,  0.43f },
		{ -0.50f,  0.00f }
	};

	b2PolygonShape shape;
	shape.Set(shape_data, 6);

	b2Fixture *fixture = body->CreateFixture(&shape, 10.0f);
	fixture->SetFriction(0.0f);
	fixture->SetRestitution(0.0f);
}

void Player::_PrepareRenderer() const {
	_PrepareShader();

	if (_is_renderer_prepared) {
		return;
	}

	// prepare the VAO/VBO for rendering
	std::vector<GLfloat> data = {
			0.25f, 0.00f,
			0.50f, 0.21f,
			0.75f, 0.00f,
			0.69f, 0.32f,
			1.00f, 0.43f,
			0.69f, 0.54f,
			0.75f, 0.86f,
			0.50f, 0.65f,
			0.25f, 0.86f,
			0.31f, 0.54f,
			0.00f, 0.43f,
			0.31f, 0.32f,
			0.25f, 0.00f
//			,
//
//			0.25f, 0.00f,
//			0.75f, 0.00f,
//			1.00f, 0.43f,
//			0.75f, 0.86f,
//			0.25f, 0.86f,
//			0.00f, 0.43f,
//			0.25f, 0.00f
	};

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
