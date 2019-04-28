/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#ifndef PLAYERCONTROLLER_H_
#define PLAYERCONTROLLER_H_

#include <set>

#include "Player.h"

class PlayerController {

public:
	PlayerController(Player &player);

	void OnKey(int key, int scancode, int action, int mods);

	void UpdatePlayer(float dt);

private:
	enum Key {
		JUMP, LEFT, RIGHT
	};

	void _HandleJumping(float dt);
	void _HandleLateral(float dt);

	bool _KeyPressed(Key key) const;

	Player &_player;
	std::set<int> _pressed_keys;

	static constexpr float _grounded_timer_value = 0.25f;
	float _grounded_timer = 0;

	static constexpr float _jump_pressed_timer_value = 0.1f;
	float _jump_pressed_timer = 0;

	static constexpr float _jump_cooldown_timer_value = 0.3f;
	float _jump_cooldown_timer = 0;

	static constexpr float _player_speed = 0.7f;
	static constexpr float _damping_grounded = 0.4f;
	static constexpr float _damping_turning = 0.7f;
	static constexpr float _damping_stopping = 0.6f;
	static constexpr float _damping_air_extra = 0.3f;

};

#endif
