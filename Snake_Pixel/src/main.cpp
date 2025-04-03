
#include <iostream>
#include <string> 

#include "frontend/window.h"
#include "backend/snake.h"
#include "backend/point.h"


int main() {


    Window screen(50);
	screen.set_box_grid(3, 3, 2);
    screen.set_box_grid(4, 4, 3);
    screen.load();

    Snake snake;
    snake.set_head_p(Point(30, 20));

    std::string a = std::to_string(snake.get_head_p().x);
    std::cout << a << std::endl;
  

}