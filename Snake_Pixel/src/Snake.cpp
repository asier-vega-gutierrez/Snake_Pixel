#pragma once

#include <memory>
#include <chrono>
#include <thread>
#include <vector>
#include <atomic>
#include <iostream>
#include "Cell.cpp"
#include "Map.cpp"

// Calss to manage snake and its movemtne
class Snake {
private:
	// Snake body
	std::vector<Cell> _body;
	// Thread-safe flag
	std::atomic<bool> _move{ false };
	// Flag to stop the thread
	std::atomic<bool> _stop{ false };
	// Direction of the snake
	int _dir_x = 2; // 0 stay, 1 positive, 2 negative
	int _dir_y = 0; // 0 stay, 1 positive, 2 negative
	// Thread for snake movement
	std::thread _movementThread;
	// Movement speed (ms)
	int _speed = 100;
	// Map object
	Map* _map;
	int _map_size = 0;
	// flag to lose
	bool lose = false;


public:
	// Default constructor
	Snake() = default;

	// Constructor
	Snake(int map_size, Map* map) :
		_map(map),
		_map_size(map_size){
		// Starting with 3 cells in the start point
		init_cells();
	}

	// Destructor
	~Snake() {
		stop_thread(); // Ensure the thread is stopped on destruction
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
	bool get_lose() {
		return lose;
	}

	// Setters
	void set_dir_x(int dir_x) {
		_dir_x = dir_x;
	}
	void set_dir_y(int dir_y) {
		_dir_x = dir_y;
	}

	// Callback function to start the snake movement
	void callback_start_snake() {
		if (_move == false && _stop == false) {
			_move = true;
			_movementThread = std::thread(&Snake::snake_loop, this);
		}
	}

	// Callback function to stop the snake movement
	void callback_pause_snake() {
		if (_move == true) {
			_move = false;
		}
	}

	// Callback function to resume the snake movement
	void callback_resume_snake() {
		if (_move == false) {
			_move = true;
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

	// Callback function to reset the snake
	void callback_reset() {
		std::cout << "Resetting snake..." << std::endl;
		// Reset the snake flags and thread
		lose = false;
		stop_thread();
		_stop = false;
		_move == false;
		// Clear the snake body
		_body.clear();
		// Reset the direction
		_dir_x = 2;
		_dir_y = 0;
		// Reset the map
		_map->set_up_cells();
		_map->set_up_borders();
		// Add initial cells
		init_cells();
	}
	
	// Function to manage snae thread end
	void stop_thread() {
		_stop = true;
		if (_movementThread.joinable()) {
			_movementThread.join(); // Wait for the thread to finish
		}
	}

	// Function to set a snake object on the grid vector
	int set_snake() {
		for (Cell cell : _body) {
			_map->set_cell(cell.get_x(), cell.get_y(), cell);
		}
		return 0;
	}


private:

	// Function to initialize the snake cells
	void init_cells() {
		Cell initialCells[3] = {
			Cell(_map_size / 2, _map_size / 2, 2, true),
			Cell(_map_size / 2 + 1, _map_size / 2, 2, true),
			Cell(_map_size / 2 + 2, _map_size / 2, 2, true)
		};
		for (int i = 0; i < 3; ++i) {
			_body.push_back(initialCells[i]);
		}
	}

	void snake_loop() {
		while (_stop == false) {
			while (_move == true) {
				// Itereate only if the tread is not stopped
				std::this_thread::sleep_for(std::chrono::milliseconds(_speed));
				// Move snake
				snake_move();
				// Check if snake losing state
				snake_check_lose();
			}
		}
	}

	// Sanke movement loop
	int snake_move() {
		// Get the actual cordinates of the snake head
		int prev_x = _body[0].get_x();
		int prev_y = _body[0].get_y();
		// Update snake head position
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
		// Move the rest of the snake body
		for (int i = 1; i < _body.size(); ++i) {
			int temp_x = _body[i].get_x();
			int temp_y = _body[i].get_y();
			_body[i].set_x(prev_x);
			_body[i].set_y(prev_y);
			prev_x = temp_x;
			prev_y = temp_y;
		}
		std::cout << "Snake moved to: " << _body[0].get_x() << ", " << _body[0].get_y() << std::endl;
		return 0;
	}

	// Function to check if the snake is losing
	bool snake_check_lose() {
		Cell actual_map_cell = _map->get_cell(_body[0].get_x(), _body[0].get_y());
		// if hit wall
		if (actual_map_cell.get_type() == 1) {
			callback_pause_snake();
			lose = true;
		}
		return true;
	}

};

