/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#ifndef WALL_H_
#define WALL_H_

#include <Box2D/Box2D.h>

#include "Entity.h"
#include "Image.h"
#include "ShaderProgram.h"

class Wall : public Entity {

public:
	Wall(unsigned x, unsigned y);
	virtual ~Wall() = default;

	virtual void Render(const glm::ivec2& screenDimensions) const;

protected:
	b2BodyDef _CreateBody() const;
	void _CreateFixture(std::shared_ptr<b2Body> body) const;

private:
	static void _PrepareRenderer();
	static void _PreparePhysics();

	static GLhandle _vao;
	static GLhandle _vbo;
	static bool _is_renderer_prepared;

	static b2PolygonShape _shape;
	static bool _is_physics_prepared;
};

#endif
