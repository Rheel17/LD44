/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#ifndef OVERLAY_H_
#define OVERLAY_H_

#include "UI.h"

class Overlay : public UI {

public:
	Overlay(Level& level);

	void Render(const glm::ivec2& screenDimensions);

private:
	Level& _level;

};

#endif
