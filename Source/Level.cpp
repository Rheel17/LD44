/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#include "Level.h"

#include <algorithm>
#include <iostream>

#include "Bullet.h"
#include "Resources.h"

class CollisionCallback : public b2ContactListener {
	void BeginContact(b2Contact *contact) {
		Entity *e1 = static_cast<Entity *>(contact->GetFixtureA()->GetBody()->GetUserData());
		Entity *e2 = static_cast<Entity *>(contact->GetFixtureB()->GetBody()->GetUserData());

		e1->OnCollisionStart(e2);
		e2->OnCollisionStart(e1);
	}

	void EndContact(b2Contact *contact) {
		Entity *e1 = static_cast<Entity *>(contact->GetFixtureA()->GetBody()->GetUserData());
		Entity *e2 = static_cast<Entity *>(contact->GetFixtureB()->GetBody()->GetUserData());

		e2->OnCollisionEnd(e1);
		e1->OnCollisionEnd(e2);
	}
};

Level::Level() :
		_camera_x(2.0f), _camera_y(4.0f) {

	_b2_world = std::shared_ptr<b2World>(new b2World(b2Vec2(0.0f, 20.0f)), [](b2World *w) {
		delete w;
	});

	_b2_contact_listener = std::make_shared<CollisionCallback>();
	_b2_world->SetContactListener(_b2_contact_listener.get());
}

void Level::AddWall(unsigned x, unsigned y) {
	_AddEntity<Wall>(x, y);
}

Shooter& Level::AddShooter(unsigned x, unsigned y, float shootTime, float currentTime) {
	return *_AddEntity<Shooter>(x, y, shootTime, currentTime);
}

void Level::CreatePlayer(float x, float y) {
	_player = _AddEntity<Player>(x, y);
	_player_controller = std::make_shared<PlayerController>(*_player);
}

void Level::SpawnBlockBullet(float x, float y, float vx, float vy) {
	_AddEntity<Bullet>(x, y, vx, vy, glm::vec4 { 1.0f, 0.0, 0.0, 1.0f });
}

void Level::Update() {
	// TODO: make this the actual delta?
	// for now, let's keep it at this, I only have 48 hours
	// to complete this.
	float dt = 1.0f / 60.0f;

	if (_player_controller) {
		// handle input
		_player_controller->UpdatePlayer(dt);
	}

	// step the physics
	_b2_world->Step(dt, 10, 10);

	// update the entities
	std::vector<std::shared_ptr<Entity>> deadEntities;

	for (auto entity : _entities) {
		entity->Update(dt, *this);

		if (!entity->IsAlive()) {
			deadEntities.push_back(entity);
		}
	}

	// remove dead entities
	for (auto dead : deadEntities) {
		_entities.erase(dead);
	}
}

void Level::Render(float dt, const glm::ivec2& screenDimensions) {
	for (std::shared_ptr<Entity> entity : _entities) {
		if (entity->IsAlive()) {
			entity->Render(screenDimensions);
		}
	}
}

void Level::OnKey(int key, int scancode, int action, int mods) {
	if (_player_controller) {
		_player_controller->OnKey(key, scancode, action, mods);
	}
}

Player& Level::GetPlayer() {
	return *_player;
}

Level Level::GenerateLevel() {
	Level level;
	const Image& levelImage = Resources::level;

	std::vector<std::reference_wrapper<Shooter>> shooters;
	std::vector<glm::ivec2> shootingDirections;

	for (unsigned i = 0; i < levelImage.Width(); i++) {
		for (unsigned j = 0; j < levelImage.Height(); j++) {
			unsigned color = levelImage.At(i, j).RGBA();

			if ((color & 0xFF) == 0) {
				continue;
			} else {
				color >>= 8;
			}

			if (color == 0x000000) {
				level.AddWall(i, j);
			} else if (color == 0xFF0000) {
				Shooter& shooter = level.AddShooter(i, j, 1.0f, -3.9f);
				shooters.push_back(shooter);
			} else if (color == 0x0000FF) {
				shootingDirections.push_back({ i, j });
			} else {
				std::cerr << "Invalid color: " << color << " @" << i << "," << j << std::endl;
			}
		}
	}

	for (const auto& direction : shootingDirections) {
		for (Shooter& shooter : shooters) {
			int dx = abs(shooter.X() - direction.x);
			int dy = abs(shooter.Y() - direction.y);

			if ((dx == 1 && dy == 0) || (dx == 0 && dy == 1)) {
				shooter.AddShootingDirection(direction.x - shooter.X(), direction.y - shooter.Y());
			}
		}
	}

	level.CreatePlayer(2.0f, 5.0f);

	return level;
}

