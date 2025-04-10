#pragma once

#include <iostream>
#include "Snake.cpp"

class Menu {
private:
	int _game_state = 0; // 0: not started, 1: pauseed, 2: runining
	// Snake object
	Snake* _snake;

public:
	// Default constructor
	Menu() = default;
	// Constructor
	Menu(Snake* snake) :
		_snake(snake){

	}

	// Callback function to handle start
	void callback_start() {
		if (_game_state == 0) {
			std::cout << "Starting game..." << std::endl;
			_game_state = 2; // Set game state to running
			_snake->callback_start_snake();
		}
		else if (_game_state == 1) {
			std::cout << "Restarting game..." << std::endl;
			_game_state = 2;
			_snake->callback_resume_snake();
		}
	}

	// Callback function to handle pause
	void callback_pause() {
		std::cout << "Pausing game..." << std::endl;
		_game_state = 1;
		_snake->callback_pause_snake();
	}





};