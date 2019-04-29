/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#ifndef GAME_H_
#define GAME_H_

#include "SoundManager.h"
#include "Overlay.h"
#include "MainMenu.h"
#include "Window.h"

class Game : public Window::EventListener {

public:
	enum State {
		MAIN_MENU,
		CREATING_LEVEL,
		PLAYING,
		GAME_OVER
	};

	Game();

	void RenderGame(float dt);
	void UpdateGame(float dt);

	void OnMouseMove(float x, float y);
	void OnMouseButton(int button, int action, int mods);
	void OnKey(int key, int scancode, int action, int mods);

private:
	Window _window;
	Renderer _renderer;
	SoundManager _sound_manager;

	State _state = MAIN_MENU;
	Level _current_level;
	std::shared_ptr<Overlay> _overlay;
	std::shared_ptr<MainMenu> _main_menu;

};

#endif
