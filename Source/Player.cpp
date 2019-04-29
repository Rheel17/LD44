/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#include "Player.h"

#include <cmath>

#include "Bullet.h"
#include "Diamond.h"
#include "Level.h"

Player::Player(float x, float y) :
		Entity(x, y, 0.0f),
		_display_bullet(std::make_shared<Bullet>(0, 0, 0, 0, glm::vec4 { 1.0f, 1.0f, 0.0f, 1.0f }, 0.5f, false, false)) {}

void Player::Render(const glm::ivec2& screenDimensions, const glm::vec3& cameraParams) const {
	_PrepareRenderer();

	_line_shader.Use();
	_line_shader["screenDimensions"] = (glm::vec2) screenDimensions;
	_line_shader["location"] = glm::vec2 { _x, _y - 0.07f };
	_line_shader["color"] = glm::vec4 { 1.0f, 0.0f, 1.0f, 1.0f };
	_line_shader["rotation"] = _rotation;
	_line_shader["rotationCenter"] = glm::vec2 { 0.50f, 0.43f };
	_line_shader["cameraParams"] = cameraParams;
	_line_shader["scale"] = 1.0f;

	glBindVertexArray(_vao);
	glDrawArrays(GL_LINE_STRIP, 0, _count);
	glBindVertexArray(0);

	// render the bullets
	float rotation = _rotation;
	for (unsigned i = 0; i < _bullet_count; i++) {
		_display_bullet->X() = _x + 0.25f * cos(rotation) + 0.25f;
		_display_bullet->Y() = _y + 0.25f * sin(rotation) - 0.25f;
		_display_bullet->Render(screenDimensions, cameraParams);

		rotation += M_PI / 3.0f;
	}
}

void Player::OnCollisionStart(Entity *other, b2Contact *contact) {
	Wall *wall = dynamic_cast<Wall *>(other);
	Bullet *bullet = dynamic_cast<Bullet *>(other);
	Diamond *diamond = dynamic_cast<Diamond *>(other);

	if (wall) {
		_touching_walls.insert(wall);
	} else if (bullet) {
		_bullet_count++;
		_hp--;

		_on_update.push_back([wall](Entity &p, Level &l) {
			l.ShakeScreen(50.0f, 0.1f);
		});
	} else if (diamond) {
		diamond->Die();

		_score++;
		_hp = max_hp;
	}

	if (_hp == 0 || _bullet_count > 6) {
		Die();
	}
}

void Player::OnCollisionEnd(Entity *other, b2Contact *contact) {
	Wall *wall = dynamic_cast<Wall *>(other);

	if (wall) {
		_touching_walls.erase(wall);
	}
}

bool Player::IsGrouded() const {
	for (Wall *wall : _touching_walls) {
		if (_y <= wall->Y() + 0.42f &&
				_x - 0.5f <= wall->X() && wall->X() <= _x + 0.5f) {
			return true;
		}
	}

	return false;
}

unsigned Player::Score() const {
	return _score;
}

unsigned Player::Health() const {
	return _hp;
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
