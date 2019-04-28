/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#ifndef GAME_H_
#define GAME_H_

#include "Level.h"
#include "SoundManager.h"
#include "Window.h"

class Game : public Window::EventListener {

public:
	enum State {
		CREATING_LEVEL,
		PLAYING
	};

	Game();

	void RenderGame(float dt);
	void UpdateGame();

	void OnMouseMove(float x, float y);
	void OnMouseButton(int button, int action, int mods);
	void OnKey(int key, int scancode, int action, int mods);

private:
	Window _window;
	Renderer _renderer;
	SoundManager _sound_manager;

	State _state;
	Level _current_level;

};

#endif
