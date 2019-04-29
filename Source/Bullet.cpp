/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#include "Bullet.h"

#include <cmath>
#include <vector>

#include "Level.h"
#include "Wall.h"

GLhandle Bullet::_vao;
GLhandle Bullet::_vbo;
unsigned Bullet::_count;
bool Bullet::_is_renderer_prepared = false;

Bullet::Bullet(float x, float y, float vx, float vy, const glm::vec4& color, float scale, bool following, bool exploding) :
		Entity(x, y, 0.0f),
		_start_vx(vx), _start_vy(vy), _color(color), _scale(scale), _following(following), _exploding(exploding) {}

void Bullet::Render(const glm::ivec2& screenDimensions, const glm::vec3& cameraParams) const {
	_PrepareRenderer();

	_line_shader.Use();
	_line_shader["screenDimensions"] = (glm::vec2) screenDimensions;
	_line_shader["location"] = glm::vec2 { _x, _y };
	_line_shader["color"] = _color;
	_line_shader["rotation"] = _rotation;
	_line_shader["rotationCenter"] = glm::vec2 { 0.5f, 0.5f };
	_line_shader["cameraParams"] = cameraParams;
	_line_shader["scale"] = _scale;

	glBindVertexArray(_vao);
	glDrawArrays(GL_LINE_LOOP, 0, _count);
	glBindVertexArray(0);
}

void Bullet::OnCollisionStart(Entity *other, b2Contact *contact) {
	Bullet *bullet = dynamic_cast<Bullet *>(other);

	if (!bullet || bullet->_color != _color || _exploding) {
		if (_exploding) {
			_on_update.push_back([](Entity& e, Level& l) {
				l.ExplodeAt(e.X(), e.Y());
				l.ShakeScreen(15.0f, 0.5f);
				l.ShakeScreen(15.0f, 0.5f);
			});
		} else {
			Die();
		}
	}
}

void Bullet::InternalUpdate(float dt, Level& level) {
	if (!_following) {
		return;
	}

	Player& player = level.GetPlayer();
	glm::vec2 delta { player.X() - _x, player.Y() - _y };

	Entity *hit = level.Raycast(
			glm::vec2 { _x, _y },
			delta,
			0.01f, 10000.0f,
			[](Entity *e) -> bool {
				return !(bool) (dynamic_cast<Bullet *>(e));
			});

	if (hit == &player) {
		float velocity = _b2_body->GetLinearVelocity().Length();
		b2Vec2 direction = _b2_body->GetLinearVelocity();
		direction.x += delta.x * 0.1f;
		direction.y += delta.y * 0.1f;
		direction.Normalize();
		direction *= velocity;

		_b2_body->SetLinearVelocity(direction);
	}
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
	shape.m_radius = 0.15f;

	b2Fixture *fixture = body->CreateFixture(&shape, 1.0f);
	fixture->SetRestitution(1.0f);
}

void Bullet::_PrepareRenderer() {
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

