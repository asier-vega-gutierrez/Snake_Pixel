

#include "Window.cpp"
#include "backend/Snake.cpp"
#include "backend/Menu.cpp"
#include "backend/Map.cpp"
#include <yaml-cpp/yaml.h>


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