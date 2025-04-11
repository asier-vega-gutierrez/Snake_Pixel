#pragma once

#include <vector>
#include "Cell.cpp"
#include "Snake.cpp"	

// Class to manage cells vector (2D map)
class Map {
private:
	// 2D vector of cells
	std::vector<std::vector<Cell>> _cells = {};
	// Grid size
	int _gridSize = {};
	// Snake object
	Snake* _snake = {};

public:
	// Default constructor
	Map() = default;
	// Constructor
	Map(int gridSize, Snake* snake) :
		_gridSize(gridSize + 1),
		_cells(gridSize + 1, std::vector<Cell>(gridSize + 1)),
		_snake(snake){

	}

	// Destructor
	~Map() {
		// Destructor logic if needed
	}

	// Function to get a cell
	Cell get_cell(int x, int y) {
		return _cells[x][y];
	}

	// Function to set a cell
	void set_cell(int x, int y, Cell cell) {
		_cells[x][y] = cell;
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

	// Function to set a snake object on the grid vector
	int set_snake() {
		for (Cell cell : _snake->get_body()) {
			_cells[cell.get_x()][cell.get_y()] = cell;
			//std::cout << "Snake cell at (" << cell.get_x() << ", " << cell.get_y() << ")" << std::endl;
		}
		return 0;
	}


};
