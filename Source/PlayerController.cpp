/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#include "PlayerController.h"

#include <GLFW/glfw3.h>

#include <cmath>

PlayerController::PlayerController(Player &player) :
		_player(player) {}

void PlayerController::OnKey(int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		_pressed_keys.insert(key);
	} else if (action == GLFW_RELEASE) {
		_pressed_keys.erase(key);
	}
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
