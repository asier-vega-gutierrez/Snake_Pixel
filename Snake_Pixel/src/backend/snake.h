#pragma once

#include <memory>
#include "backend/Point.h"

class Snake {

private:
    //Static instance
    static std::unique_ptr<Snake> instance;
    //Atributes
    Point _head_p;
    
public:
    //Contructor
    Snake() : _head_p(0, 0) {};
    //Gets
    Point get_head_p() {
        return _head_p;
    }
    //Sets
    int set_head_p(Point p) { 
        _head_p = p;
        return 0;
    }

};

std::unique_ptr<Snake> Snake::instance = nullptr;