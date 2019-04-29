/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#include "Shooter.h"

#include "Error.h"
#include "Level.h"

Shooter::Shooter(unsigned x, unsigned y, float shootTime, float currentTime) :
		Wall(x, y), _shoot_time(shootTime), _current_time(currentTime) {}

void Shooter::AddShootingDirection(int dx, int dy) {
	if (_vao) {
		throw showerror("Shooter renderer already initialized");
	}

	_shooting_directions.push_back({ dx, dy });
}

void Shooter::SetNextAsDiamond() {
	_next_diamond = true;
}

void Shooter::Render(const glm::ivec2& screenDimensions, const glm::vec3& cameraParams) const {
	_PrepareRenderer();

	Wall::Render(screenDimensions, cameraParams);
	_line_shader["color"] = glm::vec4 {
		1.0f,
		1.0f - _current_time / _shoot_time,
		1.0f - _current_time / _shoot_time,
		1.0f
	};

	glBindVertexArray(_vao);
	glDrawArrays(GL_LINES, 0, _count);
	glBindVertexArray(0);
}

void Shooter::InternalUpdate(float dt, Level& level) {
	_current_time += dt;

	std::normal_distribution<float> dist(0.0f, 2.0f);

	if (_current_time > _shoot_time) {
		_current_time = 0;

		int diamondIndex = -1;
		if (_next_diamond) {
			diamondIndex = std::uniform_int_distribution<int>(0, _shooting_directions.size() - 1)(level.RNG());
			_next_diamond = false;
		}

		int idx = 0;
		for (const auto& direction : _shooting_directions) {
			if (diamondIndex == idx) {
				level.SpawnDiamond(_x + direction.x,
						_y + direction.y,
						float(direction.x) * 0.5f + direction.y * dist(level.RNG()),
						float(direction.y) * 0.5f + direction.x * dist(level.RNG()));
			} else {
				level.SpawnBlockBullet(_x + direction.x,
						_y + direction.y,
						float(direction.x) * 2.0f + direction.y * dist(level.RNG()),
						float(direction.y) * 2.0f + direction.x * dist(level.RNG()));
			}
			idx++;
		}
	}
}

void Shooter::_PrepareRenderer() const {
	_PrepareShader();

	if (_is_renderer_prepared) {
		return;
	}

	std::vector<GLfloat> data;

	for (const auto& direction : _shooting_directions) {
		std::vector<GLfloat> add;

		if (direction == glm::ivec2 { 1, 0 }) {
			add = {
					0.95f, 0.20f, 0.60f, 0.50f,
					0.60f, 0.50f, 0.95f, 0.80f
			};
		} else if (direction == glm::ivec2 { -1, 0 }) {
			add = {
					0.05f, 0.20f, 0.40f, 0.50f,
					0.40f, 0.50f, 0.05f, 0.80f
			};
		} else if (direction == glm::ivec2 { 0, -1 }) {
			add = {
					0.20f, 0.95f, 0.50f, 0.60f,
					0.50f, 0.60f, 0.80f, 0.95f
			};
		}else if (direction == glm::ivec2 { 0, 1 }) {
			add = {
					0.20f, 0.05f, 0.50f, 0.40f,
					0.50f, 0.40f, 0.80f, 0.05f
			};
		}

		std::copy(add.begin(), add.end(), std::back_inserter(data));
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
