/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#include "Level.h"

#include <algorithm>

#include "Bullet.h"
#include "Diamond.h"
#include "Resources.h"

class CollisionCallback : public b2ContactListener {
	void BeginContact(b2Contact *contact) {
		Entity *e1 = static_cast<Entity *>(contact->GetFixtureA()->GetBody()->GetUserData());
		Entity *e2 = static_cast<Entity *>(contact->GetFixtureB()->GetBody()->GetUserData());

		e1->OnCollisionStart(e2, contact);
		e2->OnCollisionStart(e1, contact);
	}

	void EndContact(b2Contact *contact) {
		Entity *e1 = static_cast<Entity *>(contact->GetFixtureA()->GetBody()->GetUserData());
		Entity *e2 = static_cast<Entity *>(contact->GetFixtureB()->GetBody()->GetUserData());

		e2->OnCollisionEnd(e1, contact);
		e1->OnCollisionEnd(e2, contact);
	}
};

Level::Level() :
		_camera_x(12.0f), _camera_y(12.0f) {

	_b2_world = std::shared_ptr<b2World>(new b2World(b2Vec2(0.0f, 20.0f)), [](b2World *w) {
		delete w;
	});

	_b2_contact_listener = std::make_shared<CollisionCallback>();
	_b2_world->SetContactListener(_b2_contact_listener.get());
}

void Level::AddWall(unsigned x, unsigned y) {
	_AddEntity<Wall>(x, y);
}

void Level::AddShooter(unsigned x, unsigned y, float shootTime, float currentTime) {
	std::shared_ptr<Shooter> shooter = _AddEntity<Shooter>(x, y, shootTime, currentTime);
	_shooters.insert(shooter);
}

void Level::CreatePlayer(float x, float y) {
	_player = _AddEntity<Player>(x, y);
	_player_controller = std::make_shared<PlayerController>(*_player);
}

void Level::SpawnBlockBullet(float x, float y, float vx, float vy) {
	_AddEntity<Bullet>(x, y, vx, vy, glm::vec4 { 1.0f, 0.0, 0.0, 1.0f }, 1.0f, true, false);
}

void Level::SpawnPlayerBullet(float x, float y, float vx, float vy, bool exploding) {
	_AddEntity<Bullet>(x, y, vx, vy, glm::vec4 { 1.0f, 1.0, 0.0, 1.0f }, 1.0f, false, exploding);
}

void Level::SpawnDiamond(float x, float y, float vx, float vy) {
	_diamond = _AddEntity<Diamond>(x, y, vx, vy);
}

void Level::ShakeScreen(float power, float amplitude) {
	for (int i = 0; i < 4; i++) {
		ShakeScreen(glm::vec2 { _unit_distribution(_rng), _unit_distribution(_rng) / 1.7f }, power, amplitude);
	}
}

void Level::ShakeScreen(const glm::vec2& direction, float power, float amplitude) {
	_screen_shakers.insert(std::make_shared<ScreenShaker>(direction, power, 5.0f, amplitude));
}

void Level::ExplodeAt(float x, float y) {
	b2Vec2 explosion { x, y };
	float explosionDistance = 5.0f;

	for (auto entity : _entities) {
		if (!dynamic_cast<Bullet *>(entity.get())) {
			continue;
		}

		b2Vec2 position { entity->X(), entity->Y() };
		b2Vec2 distanceVector = position - explosion;

		if (distanceVector.LengthSquared() <= explosionDistance * explosionDistance) {
			entity->Die();
		}
	}
}

bool Level::Update(float dt) {
	if (_is_game_over) {
		return false;
	}

	_time += dt;

	if (_player_controller) {
		// handle input
		_player_controller->UpdatePlayer(dt);
	}

	// step the physics
	for (int i = 0; i < 5; i++) {
		_b2_world->Step(dt / 5.0f, 10, 10);
	}

	// check if a diamond needs to be spawned
	if (_diamond == nullptr && _time > 5.0f && !_has_diamond) {
		int index = std::uniform_int_distribution<int>(0, _shooters.size() - 1)(_rng);
		std::unordered_set<std::shared_ptr<Shooter>>::const_iterator it(_shooters.begin());
		std::advance(it, index);

		(*it)->SetNextAsDiamond();
		_has_diamond = true;
	}

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

		if (dead == _diamond) {
			_diamond = nullptr;
			_has_diamond = false;
		}

		if (dead == _player) {
			_is_game_over = true;
		}
	}

	return !_is_game_over;
}

void Level::Render(float dt, const glm::ivec2& screenDimensions) {
	// let camera follow player
	if (_camera_x < _player->X() - 5) { _camera_x = _player->X() - 5; }
	if (_camera_x > _player->X() + 5) { _camera_x = _player->X() + 5; }
	if (_camera_y < _player->Y() - 3) { _camera_y = _player->Y() - 3; }
	if (_camera_y > _player->Y() + 3) { _camera_y = _player->Y() + 3; }

	glm::vec2 shake(0, 0);

	// apply screen-shake
	std::vector<std::shared_ptr<ScreenShaker>> shakersToRemove;

	for (auto shaker : _screen_shakers) {
		shake += shaker->Update(dt);

		if (shaker->T() > 10) {
			shakersToRemove.push_back(shaker);
		}
	}

	for (auto shaker : shakersToRemove) {
		_screen_shakers.erase(shaker);
	}

	// render the entities
	glm::vec3 cameraParams { _camera_x + shake.x, _camera_y + shake.y, 1.75f };
//	glm::vec3 cameraParams { 5, 5, 1.75f };

	for (std::shared_ptr<Entity> entity : _entities) {
		if (entity->IsAlive()) {
			entity->Render(screenDimensions, cameraParams);
		}
	}

	if (!_player->IsAlive()) {
		_player->Render(screenDimensions, cameraParams);
	}

	// update the player controller
	if (_player_controller) {
		_player_controller->UpdateController(screenDimensions, cameraParams);
	}
}

void Level::OnKey(int key, int scancode, int action, int mods) {
	if (_player_controller) {
		_player_controller->OnKey(key, scancode, action, mods);
	}
}

void Level::OnMouseMove(float x, float y) {
	if (_player_controller) {
		_player_controller->OnMouseMove(x, y);
	}
}

void Level::OnMouseButton(int button, int action, int mods) {
	if (_player_controller) {
		_player_controller->OnMouseButton(button, action, mods);
	}
}

Entity *Level::Raycast(const glm::vec2& origin, const glm::vec2& direction, float tmin, float tmax, std::function<bool(Entity *)> predicate) {
	b2RayCastInput input;
	input.p1 = { origin.x, origin.y };
	input.p2 = input.p1 + b2Vec2 { direction.x, direction.y };
	input.maxFraction = tmax;

	float t = tmax + 1;
	Entity *hit = nullptr;

	for (auto entity : _entities) {
		Entity *e = entity.get();
		if (!predicate(e)) {
			continue;
		}

		auto body = entity->Body();

		for (const b2Fixture *fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
			b2RayCastOutput output;
			if (!fixture->RayCast(&output, input, 0)) {
				continue;
			}

			if (output.fraction < t) {
				t = output.fraction;
				hit = e;
			}
		}
	}

	return hit;
}

Player& Level::GetPlayer() {
	return *_player;
}

std::mt19937_64& Level::RNG() {
	return _rng;
}

bool Level::IsGameOver() {
	return _is_game_over;
}

Level Level::GenerateLevel() {
	Level level;
	const Image& levelImage = Resources::level;

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
				level.AddShooter(i, j, 1.0f, -3.9f);
			} else if (color == 0x0000FF) {
				shootingDirections.push_back({ i, j });
			} else {
				std::cerr << "Invalid color: " << color << " @" << i << "," << j << std::endl;
			}
		}
	}

	for (const auto& direction : shootingDirections) {
		for (auto shooter : level._shooters) {
			int dx = abs(shooter->X() - direction.x);
			int dy = abs(shooter->Y() - direction.y);

			if ((dx == 1 && dy == 0) || (dx == 0 && dy == 1)) {
				shooter->AddShootingDirection(direction.x - shooter->X(), direction.y - shooter->Y());
			}
		}
	}

	level.CreatePlayer(2.0f, 5.0f);

	return level;
}

