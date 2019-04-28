/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include <functional>

class Renderer {

public:
	using RenderFunction = std::function<void(float)>;

	/**
	 * Construct a renderer with the specified render function
	 */
	Renderer(RenderFunction renderFunction) : _render_function(renderFunction) {}

	/**
	 * Performs the rendering. This just transfers the call to the
	 * internal render function passed as a constructor argument.
	 */
	void Render(float dt) const { _render_function(dt); }

private:
	RenderFunction _render_function;

};

#endif
