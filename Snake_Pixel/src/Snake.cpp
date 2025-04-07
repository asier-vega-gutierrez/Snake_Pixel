#pragma once

#include <memory>
#include "Cell.cpp"
#include <chrono>
#include <thread>

class Snake {

private:
	// Snake body
	std::vector<Cell> _body;
	bool _move = false;
	int _dir_x = 2; // 0 stay, 1 positive, 2 negative
	int _dir_y = 0; // 0 stay, 1 positive, 2 negative

public:
	Snake() = default;
	// Constructor
	Snake(int map_size) {
		// Starting with 3 cells in the star point
		Cell initialCells[3] = {
			Cell(map_size/2, map_size/2, 2, true),
			Cell(map_size / 2 + 1, map_size / 2, 2, true),
			Cell(map_size / 2 + 2, map_size / 2, 2, true)
		};
		for (int i = 0; i < 3; ++i) {
			_body.push_back(initialCells[i]);
		}		
	}

	// Getters
	std::vector<Cell> get_body() {
		return _body;
	}
	int get_dir_x() {
		return _dir_x;
	}	
	int get_dir_y() {
		return _dir_y;
	}
	bool get_move() {
		return _move;
	}
	// Setters
	void set_dir_x(int dir_x) {
		_dir_x = dir_x;
	}
	void set_dir_y(int dir_y) {
		_dir_x = dir_y;
	}

	// Callback function to update the snake position
	void callback_start() {
		std::cout << "Starting game..." << std::endl;
		if (_move == false) {
			_move = true;
			std::thread(&Snake::snake_move_loop, this).detach();
		}
	}

	// Callback function to stop the snake movement
	void callback_pause() {
		std::cout << "Pausing game..." << std::endl;
		if (_move == true) {
			_move = false;
		}
	}

	// Callback function to set the snake direction
	void callback_set_dir(int dir_x, int dir_y) {
		if (_move == true) {
			_dir_x = dir_x;
			_dir_y = dir_y;
			std::cout << "Snake set direction to: " << dir_x << ", " << dir_y << std::endl;
		}
	}

private:

	int snake_move_loop() {
		while (_move) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
			if (_dir_x == 1) {
				_body[0].set_x(_body[0].get_x() + 1);
			}
			else if (_dir_x == 2) {
				_body[0].set_x(_body[0].get_x() - 1);
			}
			if (_dir_y == 1) {
				_body[0].set_y(_body[0].get_y() + 1);
			}
			else if (_dir_y == 2) {
				_body[0].set_y(_body[0].get_y() - 1);
			}
			std::cout << "Snake moved to: " << _body[0].get_x() << ", " << _body[0].get_y() << std::endl;
		}
		return 0;
	}


};

