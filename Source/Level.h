/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#ifndef LEVEL_H_
#define LEVEL_H_

#include <type_traits>
#include <unordered_set>
#include <random>

#include <Box2D/Box2D.h>

#include "Diamond.h"
#include "PlayerController.h"
#include "ScreenShaker.h"
#include "Shooter.h"

class Level {

public:
	Level();

	void AddWall(unsigned x, unsigned y);
	void AddShooter(unsigned x, unsigned y, float shootTime, float currentTime);
	void CreatePlayer(float x, float y);
	void SpawnBlockBullet(float x, float y, float vx, float vy);
	void SpawnPlayerBullet(float x, float y, float vx, float vy, bool exploding);
	void SpawnDiamond(float x, float y, float vx, float vy);

	void ShakeScreen(float power, float amplitude);
	void ShakeScreen(const glm::vec2& direction, float power, float amplitude);
	void ExplodeAt(float x, float y);

	void Update(float dt);
	void Render(float dt, const glm::ivec2& screenDimensions);

	void OnKey(int key, int scancode, int action, int mods);
	void OnMouseMove(float x, float y);
	void OnMouseButton(int button, int action, int mods);

	Entity *Raycast(const glm::vec2& origin, const glm::vec2& direction, float tmin, float tmax,
			std::function<bool(Entity *)> predicate = [](Entity *) -> bool { return true; });

	Player& GetPlayer();
	std::mt19937_64& RNG();

private:
	template<typename T, typename ... Args>
	std::shared_ptr<T> _AddEntity(Args ... args) {
		static_assert(std::is_base_of<Entity, T>::value, "Entity instance must derive from Entity");
		std::shared_ptr<T> entity = std::make_shared<T>(args ...);
		entity->Initialize(_b2_world);
		_entities.insert(entity);
		return entity;
	}

	std::mt19937_64 _rng;
	std::uniform_real_distribution<float> _unit_distribution;

	std::unordered_set<std::shared_ptr<Entity>> _entities;
	std::unordered_set<std::shared_ptr<Shooter>> _shooters;
	std::shared_ptr<Player> _player;
	std::shared_ptr<PlayerController> _player_controller;
	std::shared_ptr<Diamond> _diamond;
	bool _has_diamond = false;

	std::shared_ptr<b2World> _b2_world;
	std::shared_ptr<b2ContactListener> _b2_contact_listener;

	float _camera_x;
	float _camera_y;
	std::unordered_set<std::shared_ptr<ScreenShaker>> _screen_shakers;

	float _time = 0.0f;

public:
	static Level GenerateLevel();

};

#endif
