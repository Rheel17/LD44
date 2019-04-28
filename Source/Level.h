/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#ifndef LEVEL_H_
#define LEVEL_H_

#include <type_traits>
#include <unordered_set>
#include <mutex>

#include <Box2D/Box2D.h>

#include "Shooter.h"
#include "PlayerController.h"

class Level {

public:
	Level();

	void AddWall(unsigned x, unsigned y);
	Shooter& AddShooter(unsigned x, unsigned y, float shootTime, float currentTime);
	void CreatePlayer(float x, float y);
	void SpawnBlockBullet(float x, float y, float vx, float vy);

	void Update();
	void Render(float dt, const glm::ivec2& screenDimensions);

	void OnKey(int key, int scancode, int action, int mods);

	Player& GetPlayer();

private:
	template<typename T, typename ... Args>
	std::shared_ptr<T> _AddEntity(Args ... args) {
		static_assert(std::is_base_of<Entity, T>::value, "Entity instance must derive from Entity");
		std::shared_ptr<T> entity = std::make_shared<T>(args ...);
		entity->Initialize(_b2_world);
		_entities.insert(entity);
		return entity;
	}

	std::unordered_set<std::shared_ptr<Entity>> _entities;

	std::shared_ptr<Player> _player;
	std::shared_ptr<PlayerController> _player_controller;

	std::shared_ptr<b2World> _b2_world;
	std::shared_ptr<b2ContactListener> _b2_contact_listener;

	float _camera_x;
	float _camera_y;

public:
	static Level GenerateLevel();

};

#endif
