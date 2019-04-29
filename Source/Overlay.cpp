/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#include "Overlay.h"

Overlay::Overlay(Level& level) :
		_level(level) {}

void Overlay::Render(const glm::ivec2& screenDimensions) {
	_InitializeScreenDimensions(screenDimensions);

	// draw the background
	_SetColor({ 0.0f, 0.0f, 0.0f, 0.9f });
	_DrawQuad(0.0f, 0.0f, 100000.0f, 0.2f);

	// draw the score
	glm::vec4 diamondColor = glm::vec4 { 0.5764f, 0.8431f, 1.0f, 1.0f };
	_SetColor(diamondColor);
	_DrawLine(0.1f, 0.1f, 0.15f, 0.05f);
	_DrawLine(0.15f, 0.05f, 0.2f, 0.1f);
	_DrawLine(0.2f, 0.1f, 0.15f, 0.15f);
	_DrawLine(0.15f, 0.15f, 0.1f, 0.1f);

	_SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	_DrawText(std::to_string(_level.GetPlayer().Score()), 0.25f, 0.06f, 0.08f);

	// draw the healthbar
	_SetColor({ 0.8f, 0.8f, 0.8f, 0.2f });
	_DrawQuad(1.0f, 0.05f, 0.5f, 0.1f);

	_SetColor({ 1.0f, 0.0f, 1.0f, 1.0f });
	_DrawQuad(1.01f, 0.06f, (_level.GetPlayer().Health() / float(Player::max_hp)) * 0.48f, 0.08f);

	if (_level.IsGameOver()) {
		_SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		_DrawText("go", 1.6f, 0.08f, 0.08f);
		_DrawText("press", 2.4f, 0.08f, 0.06f);
	}
}
