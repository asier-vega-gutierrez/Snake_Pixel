
#include <iostream>
#include <string> 

#include "Window.cpp"
#include "Snake.cpp"



int main() {

	int map_size = 50;

    Snake* snake = new Snake(map_size);

    Window screen(map_size, snake);

    screen.load();

    delete snake;
  

}