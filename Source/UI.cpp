/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#include "UI.h"

#include "Resources.h"

GLhandle UI::_quad_vao;
GLhandle UI::_quad_vbo;
GLhandle UI::_line_vao;
GLhandle UI::_line_vbo;
ShaderProgram UI::_shader;
std::vector<GLhandle> UI::_font;
bool UI::_is_renderer_prepared = false;

void UI::_InitializeScreenDimensions(const glm::ivec2& screenDimensions) {
	_PrepareRenderer();

	_shader.Use();
	_shader["screenDimensions"] = (glm::vec2) screenDimensions;

	_screen_dimensions = screenDimensions;
}

void UI::_SetColor(const glm::vec4& color) {
	_shader.Use();
	_shader["color"] = color;

	_color = color;
}

void UI::_DrawQuad(float x, float y, float width, float height) {
	_PrepareRenderer();

	_shader.Use();
	_shader["params"] = glm::vec4 { x, y, width, height };

	glBindVertexArray(_quad_vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);
}

void UI::_DrawLine(float x1, float y1, float x2, float y2) {
	_PrepareRenderer();

	_shader.Use();
	_shader["params"] = glm::vec4 { x1, y1, x2 - x1, y2 - y1 };

	glBindVertexArray(_line_vao);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
}

void UI::_DrawText(const std::string& text, float x, float y, float height) {
	_shader.Use();
	_shader["hasTexture"] = (GLint) 1;

	for (char c : text) {
		_DrawCharacter(c, x, y, height);
		x += height * 0.8f;
	}

	_shader["hasTexture"] = (GLint) 0;
}

void UI::_DrawCharacter(char character, float x, float y, float height) {
	height /= 0.95f;

	if (character < '0' || character > '9') {
		return;
	}

	glBindTexture(GL_TEXTURE_2D, (GLuint) _font[character - '0']);

	_DrawQuad(x, y, height * 0.75f, height);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void UI::_PrepareRenderer() {
	if (_is_renderer_prepared) {
		return;
	}

	_PrepareQuad();
	_PrepareLine();
	_PrepareTextures();

	_shader.AddShaderFromFile(GL_VERTEX_SHADER, "Resources/ui.vert.glsl");
	_shader.AddShaderFromFile(GL_FRAGMENT_SHADER, "Resources/ui.frag.glsl");
	_shader.Link();

	_is_renderer_prepared = true;
}

void UI::_PrepareQuad() {
	std::vector<GLfloat> data = {
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f
	};

	_quad_vao = GL::GenVertexArray();
	_quad_vbo = GL::GenBuffer();

	glBindVertexArray(_quad_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _quad_vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *) 0);

	glBindVertexArray(0);
}

void UI::_PrepareLine() {
	std::vector<GLfloat> data = {
		0.0f, 0.0f,
		1.0f, 1.0f,
	};

	_line_vao = GL::GenVertexArray();
	_line_vbo = GL::GenBuffer();

	glBindVertexArray(_line_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _line_vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *) 0);

	glBindVertexArray(0);
}

void UI::_PrepareTextures() {
	_font.push_back(_GetTexture(Resources::font0));
	_font.push_back(_GetTexture(Resources::font1));
	_font.push_back(_GetTexture(Resources::font2));
	_font.push_back(_GetTexture(Resources::font3));
	_font.push_back(_GetTexture(Resources::font4));
	_font.push_back(_GetTexture(Resources::font5));
	_font.push_back(_GetTexture(Resources::font6));
	_font.push_back(_GetTexture(Resources::font7));
	_font.push_back(_GetTexture(Resources::font8));
	_font.push_back(_GetTexture(Resources::font9));
}

GLhandle UI::_GetTexture(const Image& image) {
	// create the OpenGL texture
	GLhandle texture = GL::GenTexture();

	// bind the texture
	glBindTexture(GL_TEXTURE_2D, (GLuint) texture);

	// upload texture data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.Width(), image.Height(), 0, GL_RGBA, GL_FLOAT, image.Data());

	// set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// unbind the texture and return it
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}
