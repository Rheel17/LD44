/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#include "Game.h"

#include <GLFW/glfw3.h>

#include <chrono>

Game::Game() :
		_window(this),
		_renderer([this](float dt) { RenderGame(dt); }) {

	_window.Show(_renderer);
}

void Game::RenderGame(float dt) {
	if (!_main_menu) {
		_main_menu = std::make_shared<MainMenu>();
	}

	UpdateGame(dt);

	switch (_state) {
		case MAIN_MENU:
			_main_menu->Render({ _window.Width(), _window.Height() });
			break;
		case CREATING_LEVEL:
			break;
		case PLAYING:
		case GAME_OVER:
			_current_level.Render(dt, { _window.Width(), _window.Height() });
			_overlay->Render({ _window.Width(), _window.Height() });
			break;
	}
}

void Game::UpdateGame(float dt) {
	switch (_state) {
		case CREATING_LEVEL:
			_current_level = Level::GenerateLevel();
			_sound_manager.PlayBackground();
			_overlay = std::make_shared<Overlay>(_current_level);
			_state = PLAYING;
			break;
		case PLAYING:
			if (!_current_level.Update(dt)) {
				_state = GAME_OVER;
				_sound_manager.StopBackground();
			}
			break;
		case GAME_OVER:
			break;
		default:
			break;
	}
}

void Game::OnMouseMove(float x, float y) {
	switch (_state) {
		case PLAYING:
			_current_level.OnMouseMove(x, y);
			break;
		default:
			break;
	}
}

void Game::OnMouseButton(int button, int action, int mods) {
	switch (_state) {
		case PLAYING:
			_current_level.OnMouseButton(button, action, mods);
			break;
		default:
			break;
	}
}

void Game::OnKey(int key, int scancode, int action, int mods) {
	switch (_state) {
		case PLAYING:
			_current_level.OnKey(key, scancode, action, mods);
			break;
		case MAIN_MENU:
			if (key == GLFW_KEY_ENTER || key == GLFW_KEY_KP_ENTER) {
				_state = CREATING_LEVEL;
			}
			break;
		case GAME_OVER:
			if (key == GLFW_KEY_ESCAPE) {
				_state = MAIN_MENU;
			}
			break;
		default:
			break;
	}
}

int main() {
	Game game;
}
