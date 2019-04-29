/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#include "MainMenu.h"

void MainMenu::Render(const glm::ivec2& screenDimensions) {
	_InitializeScreenDimensions(screenDimensions);

	float size = (800.0f / float(screenDimensions.x)) * (float(screenDimensions.x) / (screenDimensions.y));
	float x = float(screenDimensions.x) / (screenDimensions.y);
	x -= size;

	_DrawText("main", x, 0, 2 * size);
}
