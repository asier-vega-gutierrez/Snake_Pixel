
#include <iostream>
#include <string> 

#include "Window.cpp"
#include "Snake.cpp"
#include "Menu.cpp"
#include "Map.cpp"



int main() {

	int map_size = 50;
	
	Map* map = new Map(map_size);

    Snake* snake = new Snake(map_size, map);

	Menu* menu = new Menu(snake);

    Window screen(map_size, snake, menu, map);

    screen.load();

    delete menu;
    delete snake;
    delete map;

}