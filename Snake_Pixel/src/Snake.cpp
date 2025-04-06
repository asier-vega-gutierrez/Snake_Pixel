#pragma once

#include <memory>
#include "Cell.cpp"


class Snake {

private:
	// Snake body
	std::vector<Cell> _body;
    
public:
	Snake() = default;
	// Constructor
	Snake(int map_size) {
		// Starting with 3 cells in the star point
		Cell initialCells[3] = {
			Cell(map_size/2, map_size/2, 2),
			Cell(map_size / 2 + 1, map_size / 2, 2),
			Cell(map_size / 2 + 2, map_size / 2, 2)
		};
		for (int i = 0; i < 3; ++i) {
			_body.push_back(initialCells[i]);
		}		
	}

	// Getters
	std::vector<Cell> get_body() {
		return _body;
	}

	// Callback function to update the snake position
	void callback_start() {
		std::cout << "Snake callback start" << std::endl;

	}


};

