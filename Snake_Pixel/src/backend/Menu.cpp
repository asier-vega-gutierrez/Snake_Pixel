#pragma once

#include <iostream>
#include "Snake.cpp"

class Menu {
private:
	int _game_state = 0; // 0: not started, 1: pauseed, 2: runining, 3:need reset
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

	// Function to control de snake lose
	bool check_lose_state() {
		if (_snake->get_lose() == true) {
			std::cout << "Game Over, score is: " << _snake->get_food_counter() << std::endl;
			_game_state = 3;
			return true;
		}
		return false;
	}

	// Callback function to handle reset
	void callback_reset() {
		std::cout << "Resetting game..." << std::endl;
		_game_state = 0;
		_snake->callback_reset();
	}


};