#pragma once
//Class to manege boxes on the grid
class Cell {
	private:
		// Atributes
		int _x = {};
		int _y = {};
		int _type = {};

    public:
		// Default constructor
		Cell() : _x(0), _y(0), _type(0) {}
		// Constructor
		Cell(int x, int y, int type) :
			_x(x), _y(y), _type(type) {
		}

		//Getters
		int get_x() {
			return _x;
		}
		int get_y() {
			return _y;
		}
		int get_type() {
			return _type;
		}

		//Setters
		void set_x(int x) {
			_x = x;
		}
		void set_y(int y) {
			_y = y;
		}
		void set_type(int type) {
			_type = type;
		}

};