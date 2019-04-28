/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#include "Wall.h"

#include <vector>

#include "Resources.h"

GLhandle Wall::_vao;
GLhandle Wall::_vbo;
bool Wall::_is_renderer_prepared = false;

b2PolygonShape Wall::_shape;
bool Wall::_is_physics_prepared = false;

Wall::Wall(unsigned x, unsigned y) :
		Entity(float(x), float(y), 0) {}

void Wall::Render(const glm::ivec2& screenDimensions, const glm::vec3& cameraParams) const {
	_PrepareRenderer();

	_line_shader.Use();
	_line_shader["screenDimensions"] = (glm::vec2) screenDimensions;
	_line_shader["location"] = glm::vec2 { _x, _y };
	_line_shader["color"] = glm::vec4 { 1.0f, 1.0f, 1.0f, 1.0f };
	_line_shader["rotation"] = _rotation;
	_line_shader["cameraParams"] = cameraParams;

	glBindVertexArray(_vao);
	glDrawArrays(GL_LINE_LOOP, 0, 8);
	glBindVertexArray(0);
}

b2BodyDef Wall::_CreateBody() const {
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	return bodyDef;
}

void Wall::_CreateFixture(std::shared_ptr<b2Body> body) const {
	_PreparePhysics();

	b2Fixture *fixture = body->CreateFixture(&_shape, 0);
	fixture->SetFriction(0.0f);
}

void Wall::_PrepareRenderer() {
	_PrepareShader();

	if (_is_renderer_prepared) {
		return;
	}

	// prepare the VAO/VBO for rendering
	std::vector<GLfloat> data = {
			0.05f, 0.10f,
			0.10f, 0.05f,
			0.90f, 0.05f,
			0.95f, 0.10f,
			0.95f, 0.90f,
			0.90f, 0.95f,
			0.10f, 0.95f,
			0.05f, 0.90f
	};

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

void Wall::_PreparePhysics() {
	if (_is_physics_prepared) {
		return;
	}

	const float edge = 0.0f;
	const float ybias = 0.0f;

	// create the shape
	b2Vec2 shape_data[] = {
		{ -0.45f       , -0.45f + edge + ybias },
		{ -0.45f + edge, -0.45f        + ybias },
		{  0.45f - edge, -0.45f        + ybias },
		{  0.45f       , -0.40f + edge + ybias },
		{  0.45f       ,  0.45f - edge + ybias },
		{  0.45f - edge,  0.45f        + ybias },
		{ -0.45f + edge,  0.45f        + ybias },
		{ -0.45f       ,  0.45f - edge + ybias }
	};

	_shape.Set(shape_data, 8);

	_is_physics_prepared = true;
}
