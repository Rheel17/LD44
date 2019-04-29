/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#ifndef BULLET_H_
#define BULLET_H_

#include "Entity.h"

class Bullet : public Entity {

public:
	Bullet(float x, float y, float vx, float vy, const glm::vec4& color, float scale, bool following, bool exploding);

	void Render(const glm::ivec2& screenDimensions, const glm::vec3& cameraParams) const;
	void InternalUpdate(float dt, Level& level);

	void OnCollisionStart(Entity *other, b2Contact *contact);

protected:
	b2BodyDef _CreateBody() const;
	void _CreateFixture(std::shared_ptr<b2Body> body) const;

private:
	float _start_vx;
	float _start_vy;
	glm::vec4 _color;
	float _scale;
	bool _following;
	bool _exploding;

private:
	static void _PrepareRenderer();

	static GLhandle _vao;
	static GLhandle _vbo;
	static unsigned _count;
	static bool _is_renderer_prepared;

};

#endif
