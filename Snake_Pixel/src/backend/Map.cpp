#pragma once

#include <vector>
#include <random> 
#include <iostream>
#include "Cell.cpp"


// Class to manage cells vector (2D map)
class Map {
private:
	// 2D vector of cells
	std::vector<std::vector<Cell>> _cells = {};
	// Grid size
	int _gridSize = {};
	// Food flag to generate food only once
	bool _food_flag = false;

public:
	// Default constructor
	Map() = default;
	// Constructor
	Map(int gridSize) :
		_gridSize(gridSize + 1),
		_cells(gridSize + 1, std::vector<Cell>(gridSize + 1)){

	}

	// Function to get a cell
	Cell get_cell(int x, int y) {
		return _cells[x][y];
	}

	// Function to set a cell
	void set_cell(int x, int y, Cell cell) {
		_cells[x][y] = cell;
	}
	void set_food_flag(bool food_flag) {
		_food_flag = food_flag;
	}

	// Function to set up cell to type 0 (empty)
	int set_up_cells() {
		// Set all cells to type 0 (empty)
		for (int i = 0; i < _gridSize; ++i) {
			for (int j = 0; j < _gridSize; ++j) {
				Cell cell(i, j, 0, true);
				_cells[i][j] = cell;
			}
		}
		return 0;
	}

	// Function to set up map bordes type 1 (border)
	int set_up_borders() {
		// Set borders of the grid to cell type 1
		for (int i = 0; i < _gridSize; ++i) {
			// Top border
			Cell cell_h_top(i, 0, 1, false);
			_cells[i][0] = cell_h_top;
			// Bottom border
			Cell cell_h_bottom(i, _gridSize - 2, 1, false);
			_cells[i][_gridSize - 1] = cell_h_bottom;
			// Left border
			Cell cell_v_left(0, i, 1, false);
			_cells[0][i] = cell_v_left;
			// Right border
			Cell cell_v_right(_gridSize - 2, i, 1, false);
			_cells[_gridSize - 2][i] = cell_v_right;
		}
		return 0;
	}

	// Function to set all editable cells to type 0 (empty)
	int set_cells_empty() {
		// Set al editable cells to type 0 (empty)
		for (int i = 0; i < _gridSize; ++i) {
			for (int j = 0; j < _gridSize; ++j) {
				Cell cell(i, j, 0, true);
				if (_cells[i][j].get_editable() == true) {
					_cells[i][j] = cell;
				}
			}
		}
		return 0;
	}

	void set_food() {
		if (_food_flag == false) {
			// Create a random number generator
			std::random_device rd; // Seed
			std::mt19937 gen(rd()); // Mersenne Twister engine
			std::uniform_int_distribution<> dis(1, _gridSize - 2); // Range: avoid borders

			int x, y;
			do {
				x = dis(gen); // Generate random x
				y = dis(gen); // Generate random y
				// Ensure the cell can be placed
			} while (_cells[x][y].get_type() != 0 && _cells[x][y].get_type() != 1 && _cells[x][y].get_type() != 2); 

			// Set the food cell
			Cell foodCell(x, y, 3, false); // Type 3 for food
			set_cell(x, y, foodCell);
			_food_flag = true;
			std::cout << "Food set at: (" << x << ", " << y << ")" << std::endl;
			
		}
	}
};
