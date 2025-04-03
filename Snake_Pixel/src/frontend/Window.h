#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Cell.h"

// Declare the shader source code as extern
extern const char* vertexShaderSource;
extern const char* fragmentShaderSource;

// Class to manage the game window and its graphics
class Window {
private:
    // Attributes
    GLFWwindow* window = {};
    int height = 800;
    int width = 800;
    const char* title = "actual screen";
    // Grid size is from 0 to n so add 1 for better user experience
    int _gridSize = {};
    // Calculate the size of each cell in the grid
    const float _cellSize = {};
    // All filled cells on grid 
    std::vector<std::vector<Cell>> _cells = {};

public:
    // Constructor
    Window(int gridSize);

	// Function prototypes
    int load();
    int terminate();
    int set_box_grid(int x, int y, int type);

private:
    // Helper methods
    GLfloat* generate_grid();
    GLfloat* generate_box(float offsetX, float offsetY);
    std::vector<GLfloat> generate_grid_boxes_vertices(int selectedType);
    int setUpCells();

    // Static key callback function
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};
