
#include <iostream>
#include <string> 

#include "Window.cpp"
#include "Snake.cpp"
#include "Menu.cpp"



int main() {

	int map_size = 50;

    Snake* snake = new Snake(map_size);

	Menu* menu = new Menu(snake);

    Window screen(map_size, snake, menu);

    screen.load();

    delete menu;
    delete snake;

}