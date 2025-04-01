
#include "frontend/window.h"
#include "backend/snake.h"
#include <iostream>
#include <string> 

#include "backend/point.h"


int main() {


    Window screen(10);
	screen.set_box_grid(0, 0, 1);
    screen.load();

    Snake snake;
    snake.set_head_p(Point(30, 20));

    std::string a = std::to_string(snake.get_head_p().x);
    std::cout << a << std::endl;

    

  

}