/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#include "Diamond.h"

#include <cmath>

Diamond::Diamond(float x, float y, float vx, float vy) :
		Entity(x, y, 0.0f),
		_start_vx(vx), _start_vy(vy) {}

void Diamond::Render(const glm::ivec2& screenDimensions, const glm::vec3& cameraParams) const {
	_PrepareRenderer();

	_line_shader.Use();
	_line_shader["screenDimensions"] = (glm::vec2) screenDimensions;
	_line_shader["location"] = glm::vec2 { _x + 0.35f, _y - 0.35f };
	_line_shader["color"] = glm::vec4 { 0.5764f, 0.8431f, 1.0f, 1.0f };
	_line_shader["rotation"] = (float) (_rotation + M_PI / 4);
	_line_shader["rotationCenter"] = glm::vec2 { 0.15f, 0.15f };
	_line_shader["cameraParams"] = cameraParams;
	_line_shader["scale"] = 1.0f;

	glBindVertexArray(_vao);
	glDrawArrays(GL_LINE_LOOP, 0, _count);
	glBindVertexArray(0);
}


b2BodyDef Diamond::_CreateBody() const {
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.fixedRotation = true;
	body.linearVelocity.x = _start_vx;
	body.linearVelocity.y = _start_vy;
	return body;
}

void Diamond::_CreateFixture(std::shared_ptr<b2Body> body) const {
	b2PolygonShape shape;
	shape.SetAsBox(0.25f, 0.25f);

	b2Fixture *fixture = body->CreateFixture(&shape, 1.0f);
	fixture->SetRestitution(0.0f);
	fixture->SetFriction(1.0f);
}

void Diamond::_PrepareRenderer() const {
	_PrepareShader();

	if (_is_renderer_prepared) {
		return;
	}

	// prepare the VAO/VBO for rendering
	std::vector<GLfloat> data {
		0.0f, 0.0f,
		0.0f, 0.4f,
		0.4f, 0.4f,
		0.4f, 0.0f
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
