
#include <iostream>
#include <string> 

#include "Window.cpp"
#include "Snake.cpp"



int main() {


    Window screen(50);
	screen.set_box_grid(3, 3, 2);
    screen.set_box_grid(4, 4, 3);
    screen.load();

    Snake snake;
  

}