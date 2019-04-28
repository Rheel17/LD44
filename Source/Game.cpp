/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#include "Game.h"

#include <iostream>
#include <chrono>

Game::Game() :
		_window(this),
		_renderer([this](float dt) { RenderGame(dt); }),
		_state(CREATING_LEVEL) {

	_window.Show(_renderer);
}

void Game::RenderGame(float dt) {
	UpdateGame();

	switch (_state) {
		case CREATING_LEVEL:
			break;
		case PLAYING:
			_current_level.Render(dt, { _window.Width(), _window.Height() });
			break;
	}
}

void Game::UpdateGame() {
	switch (_state) {
		case CREATING_LEVEL:
			_current_level = Level::GenerateLevel();
			std::clog << "Level generated" << std::endl;
			_sound_manager.PlayBackground();
			_state = PLAYING;
			break;
		case PLAYING:
			_current_level.Update();
			break;
	}
}

void Game::OnMouseMove(float x, float y) {

}

void Game::OnMouseButton(int button, int action, int mods) {

}

void Game::OnKey(int key, int scancode, int action, int mods) {
	switch (_state) {
		case CREATING_LEVEL:
			break;
		case PLAYING:
			_current_level.OnKey(key, scancode, action, mods);
			break;
	}
}

int main() {
	Game game;
}
