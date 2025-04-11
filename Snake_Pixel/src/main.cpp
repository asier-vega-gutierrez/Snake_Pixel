
#include <iostream>
#include <string> 

#include "Window.cpp"
#include "Snake.cpp"
#include "Menu.cpp"
#include "Map.cpp"



int main() {

	int map_size = 50;

    Snake* snake = new Snake(map_size);

	Menu* menu = new Menu(snake);

	Map* map = new Map(map_size, snake);

    Window screen(map_size, snake, menu, map);

    screen.load();

    delete menu;
    delete snake;

}