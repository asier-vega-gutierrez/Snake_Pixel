#pragma once
class Box {
	public:
		int _x = {};
		int _y = {};
		int _type = {};

		Box() : _x(0), _y(0), _type(0) {}

		Box(int x, int y, int type) :
			_x(x), _y(y), _type(type) {
		}

};