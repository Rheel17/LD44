/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#ifndef MAINMENU_H_
#define MAINMENU_H_

#include "UI.h"

class MainMenu : public UI {

public:
	void Render(const glm::ivec2& screenDimensions);

};

#endif
