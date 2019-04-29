/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#ifndef UI_H_
#define UI_H_

#include <string>

#include "ShaderProgram.h"
#include "Level.h"

class UI {

public:
	virtual ~UI() = default;
	virtual void Render(const glm::ivec2& screenDimensions) = 0;

protected:
	void _InitializeScreenDimensions(const glm::ivec2& screenDimensions);
	void _SetColor(const glm::vec4& color);
	void _DrawQuad(float x, float y, float width, float height);
	void _DrawLine(float x1, float y1, float x2, float y2);
	void _DrawText(const std::string& text, float x, float y, float height);
	void _DrawCharacter(char character, float x, float y, float height);

private:
	glm::vec2 _screen_dimensions;
	glm::vec4 _color;

private:
	static void _PrepareRenderer();
	static void _PrepareQuad();
	static void _PrepareLine();
	static void _PrepareTextures();
	static GLhandle _GetTexture(const Image& image);

	static GLhandle _quad_vao;
	static GLhandle _quad_vbo;

	static GLhandle _line_vao;
	static GLhandle _line_vbo;

	static ShaderProgram _shader;

	static std::vector<GLhandle> _font;
	static GLhandle _fontgo;
	static GLhandle _fontpress;
	static GLhandle _fontmain;

	static bool _is_renderer_prepared;

};

#endif
