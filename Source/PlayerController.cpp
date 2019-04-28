/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#include "PlayerController.h"

#include <GLFW/glfw3.h>

#include <cmath>

#include "Level.h"

PlayerController::PlayerController(Player &player) :
		_player(player) {}

void PlayerController::OnKey(int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		_pressed_keys.insert(key);
	} else if (action == GLFW_RELEASE) {
		_pressed_keys.erase(key);
	}
}

void PlayerController::OnMouseMove(float x, float y) {
	_mouse_position = { x, y };
}

void PlayerController::OnMouseButton(int button, int action, int mods) {
	if (_player._bullet_count == 0 || !(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)) {
		return;
	}

	glm::vec2 pos = _mouse_position;

	// to ndc
	pos /= _screen_dimensions;
	pos *= 2;
	pos -= 1;

	// reverse camera transformation
	pos.y *= float(_screen_dimensions.y) / float(_screen_dimensions.x);
	pos *= 12.0f;
	pos.x += _camera_params.x - 0.5f;
	pos.y += _camera_params.y;

	bool exploding = abs(_player._b2_body->GetAngularVelocity()) > 15;

	// spawn the bullet on next update
	_player._on_update.push_back([pos, exploding](Entity& e, Level& l) {
		glm::vec2 direction = pos;
		direction.x -= e.X();
		direction.y -= e.Y() - 0.5f;
		direction /= sqrt(direction.x * direction.x + direction.y * direction.y);

		l.SpawnPlayerBullet(e.X() + direction.x * 0.8f, e.Y() + direction.y * 0.8f, direction.x * 10.0f, direction.y * 10.0f, exploding);
	});

	_player._bullet_count--;
}

void PlayerController::UpdateController(const glm::ivec2& screenDimensions, const glm::vec3& cameraParams) {
	_screen_dimensions = screenDimensions;
	_camera_params = cameraParams;
}

void PlayerController::UpdatePlayer(float dt) {
	_HandleJumping(dt);
	_HandleLateral(dt);
}

void PlayerController::_HandleJumping(float dt) {
	_grounded_timer -= dt;
	_jump_pressed_timer -= dt;
	_jump_cooldown_timer -= dt;

	if (_player.IsGrouded()) {
		_grounded_timer = _grounded_timer_value;
	}

	if (_KeyPressed(JUMP)) {
		_jump_pressed_timer = _jump_pressed_timer_value;
	}

	if (_grounded_timer > 0 && _jump_pressed_timer > 0 && _jump_cooldown_timer < 0) {
		_player._b2_body->SetLinearVelocity(b2Vec2(_player._b2_body->GetLinearVelocity().x, -10.0));
		_grounded_timer = 0;
		_jump_pressed_timer = 0;
		_jump_cooldown_timer = _jump_cooldown_timer_value;
	}
}

void PlayerController::_HandleLateral(float dt) {
	float velocity = _player._b2_body->GetLinearVelocity().x;

	float addedVelocity = 0.0f;

	if (_KeyPressed(LEFT))  { addedVelocity -= _player_speed; }
	if (_KeyPressed(RIGHT)) { addedVelocity += _player_speed; }

	auto direction = signbit(velocity);

	// apply the controls
	b2Vec2 point = _player._b2_body->GetWorldCenter();
	point.y -= 0.05f;

	_player._b2_body->ApplyLinearImpulse(b2Vec2(addedVelocity * _player._b2_body->GetMass(), 0), point, true);
	velocity = _player._b2_body->GetLinearVelocity().x;

	if (addedVelocity < 0.001f && addedVelocity > -0.001f) {
		velocity *= std::pow(1.0f - _damping_stopping, dt * 10.0f);
	} else if (signbit(addedVelocity) != direction) {
		velocity *= std::pow(1.0f - _damping_turning, dt * 10.0f);
	} else {
		velocity *= std::pow(1.0f - _damping_grounded, dt * 10.0f);
	}

	if (!_player.IsGrouded()) {
		velocity *= std::pow(1.0f - _damping_air_extra, dt * 10.0f);
	}

	_player._b2_body->SetLinearVelocity(b2Vec2(velocity, _player._b2_body->GetLinearVelocity().y));
}

bool PlayerController::_KeyPressed(Key key) const {
	switch (key) {
		case JUMP:
			return _pressed_keys.find(GLFW_KEY_W) != _pressed_keys.end() ||
					_pressed_keys.find(GLFW_KEY_UP) != _pressed_keys.end() ||
					_pressed_keys.find(GLFW_KEY_SPACE) != _pressed_keys.end();
		case LEFT:
			return _pressed_keys.find(GLFW_KEY_A) != _pressed_keys.end() ||
					_pressed_keys.find(GLFW_KEY_LEFT) != _pressed_keys.end();
		case RIGHT:
			return _pressed_keys.find(GLFW_KEY_D) != _pressed_keys.end() ||
					_pressed_keys.find(GLFW_KEY_RIGHT) != _pressed_keys.end();
	}

	return false;
}
