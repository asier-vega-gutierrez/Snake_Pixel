#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "frontend/shader.h"
#include "frontend/VAO.h"
#include "frontend/VBO.h"
#include "frontend/EBO.h"
#include "frontend/gladcode.h"
#include "backend/Cell.cpp"
#include "backend/Snake.cpp"
#include "backend/Menu.cpp"
#include "backend/Map.cpp"



// Class to manage the game window and its graphics
class Window {
private:
    // Attributes
    GLFWwindow* _window = {};
    int height = 800;
    int width = 800;
    const char* title = "actual screen";
    // Grid size is from 0 to n so add 1 for better user experience
    int _gridSize = {};
    // Calculate the size of each cell in the grid
    const float _cellSize = {};
    // Flag to indicate when the screen needs to be updated
    bool updateWindow = false;
	// Snake object
    Snake* _snake = {};
	// Menu object
	Menu* _menu = {};
    // map object
    Map* _map = {};

    // Vertices for cells in grid (can be updatables)
	std::vector<GLfloat> gridBoxesVerticesType1 = {};
    std::vector<GLfloat> gridBoxesVerticesType2 = {};
    std::vector<GLfloat> gridBoxesVerticesType3 = {};


public:

	//Default constructor
	Window() = default;

    // Constructor
    Window(int gridSize, Snake* snake, Menu* menu, Map* map) :
        _gridSize(gridSize + 1),
        _cellSize(2.0f / (gridSize)),
        //_cells(gridSize + 1, std::vector<Cell>(gridSize + 1)),
        _snake(snake),
        _menu(menu),
        _map(map){

        // Configure glfw
        glfwInit();
        // Using OpenGL 4.6
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
        // Using core profile 
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        // Set default map borders
        _map->set_up_cells();
		_map->set_up_borders();
    }

    // Destructor 
    ~Window() {
        terminate();
    }

	// Function to load the window
    int load() {
        // CONFIGURE BASE WINDOW
        
        // Disable window resizing
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        
        // Create glfw window object
        _window = glfwCreateWindow(width, height, title, NULL, NULL);
        if (_window == NULL) {
            std::cout << "ERROR could not create GLFW window" << std::endl;
            terminate();
            return -1;
        }
        // Set the user pointer to this instance
        glfwSetWindowUserPointer(_window, this);
        // Establish as current
        glfwMakeContextCurrent(_window);
        // Set the key callback function
        glfwSetKeyCallback(_window, key_callback);
		// Set the window close callback function
        glfwSetWindowCloseCallback(_window, window_close_callback);
        // Load opengl
        gladLoadGL();
        // Define the opengl working zone
        glViewport(0, 0, width, height);
        // Generates Shader object using shader files
        Shader shaderProgram(vertexShaderSource, fragmentShaderSource);

        // GRID
        // Generates Vertex Array Object and binds it
        VAO gridVAO;
        gridVAO.Bind();
        // Generate grid vertices
        GLfloat* gridVertices = generate_grid();
        // Generates Vertex Buffer Object and links it to vertices
        VBO gridVBO(gridVertices, sizeof(GLfloat) * _gridSize * 4 * 3);
        // Links VBO to VAO
        gridVAO.LinkVBO(gridVBO, 0);
        // Unbind all to prevent accidentally modifying them
        gridVAO.Unbind();
        gridVBO.Unbind();

        // BOXES
        // Initialize VAOs and VBOs for the boxes
        VAO boxVAOType1, boxVAOType2, boxVAOType3;
		VBO boxVBOType1(nullptr, 0), boxVBOType2(nullptr, 0), boxVBOType3(nullptr, 0);

        // Manage all events of the window while it is up
        while (!glfwWindowShouldClose(_window)) {

			// Call update to update the window with ne information
            update(boxVAOType1, boxVBOType1, boxVAOType2, boxVBOType2, boxVAOType3, boxVBOType3);

            // Specify the color of the background
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            // Clean the back buffer and assign the new color to it
            glClear(GL_COLOR_BUFFER_BIT);
            // Tell OpenGL which Shader Program we want to use
            shaderProgram.Activate();

            // Draw grid lines
            shaderProgram.setColor(1.0f, 1.0f, 1.0f);
            gridVAO.Bind();
            glDrawArrays(GL_LINES, 0, _gridSize * 4);
            // Draw type 1 vertices as triangles
            shaderProgram.setColor(1.0f, 0.0f, 0.0f);
            boxVAOType1.Bind();
            glDrawArrays(GL_TRIANGLES, 0, gridBoxesVerticesType1.size() / 3);
            // Draw type 2 vertices as triangles
            shaderProgram.setColor(0.0f, 1.0f, 0.0f);
            boxVAOType2.Bind();
            glDrawArrays(GL_TRIANGLES, 0, gridBoxesVerticesType2.size() / 3);
            // Draw type 3 vertices as triangles
            shaderProgram.setColor(0.0f, 0.0f, 1.0f);
            boxVAOType3.Bind();
            glDrawArrays(GL_TRIANGLES, 0, gridBoxesVerticesType3.size() / 3);

            // Swap the back buffer with the front buffer
            glfwSwapBuffers(_window);
            // Take care of all GLFW events
            glfwPollEvents();
        }

        // Delete all the objects we've created
        gridVAO.Delete();
        gridVBO.Delete();
        boxVAOType1.Delete();
        boxVAOType1.Delete();
        boxVAOType2.Delete();
        boxVAOType2.Delete();
        boxVAOType3.Delete();
        boxVAOType3.Delete();
        shaderProgram.Delete();

        // Empty memory
        delete[] gridVertices;

        return 0;
    }

    // Function to terminate the window
    int terminate() {
        std::cout << "Closing window..." << std::endl;
        glfwSetWindowShouldClose(_window, true);
        std::cout << "Terminating..." << std::endl;
        glfwDestroyWindow(_window);
        glfwTerminate();
        return 0;
    }

private:

    // Function to generate grid vertices (horizontal and vertical lines)
    GLfloat* generate_grid() {
        GLfloat* vertices = new GLfloat[_gridSize * 4 * 3]();
        int index = 0;
        for (int i = 0; i < _gridSize; i++) {
            float coord = (float(i) / (_gridSize - 1)) * 2.0f - 1.0f;
            // Vertical lines
            vertices[index++] = coord;
            vertices[index++] = -1.0f;
            vertices[index++] = 0.0f;
            vertices[index++] = coord;
            vertices[index++] = 1.0f;
            vertices[index++] = 0.0f;
            // Horizontal lines
            vertices[index++] = -1.0f;
            vertices[index++] = coord;
            vertices[index++] = 0.0f;
            vertices[index++] = 1.0f;
            vertices[index++] = coord;
            vertices[index++] = 0.0f;
        }
        return vertices;
    }

    // Function to generate box vertices based on the cell size and offset
    GLfloat* generate_box(float offsetX, float offsetY) {
        // Define the offset for the box position
        // Define box vertices based on the cell size and offset
        GLfloat* vertices = new GLfloat[6 * 3]();
        // First triangle
        vertices[0] = -_cellSize / 2 + offsetX;
        vertices[1] = -_cellSize / 2 - offsetY;
        vertices[2] = 0.0f;
        vertices[3] = _cellSize / 2 + offsetX;
        vertices[4] = -_cellSize / 2 - offsetY;
        vertices[5] = 0.0f;
        vertices[6] = _cellSize / 2 + offsetX;
        vertices[7] = _cellSize / 2 - offsetY;
        vertices[8] = 0.0f;
        // Second triangle
        vertices[9] = _cellSize / 2 + offsetX;
        vertices[10] = _cellSize / 2 - offsetY;
        vertices[11] = 0.0f;
        vertices[12] = -_cellSize / 2 + offsetX;
        vertices[13] = _cellSize / 2 - offsetY;
        vertices[14] = 0.0f;
        vertices[15] = -_cellSize / 2 + offsetX;
        vertices[16] = -_cellSize / 2 - offsetY;
        vertices[17] = 0.0f;
        return vertices;
    }

    // Function to generate grid filled box vertices based on the cell size and offset
    std::vector<GLfloat> generate_grid_boxes_vertices(int selectedType) {
        std::vector<GLfloat> gridBoxesVertices = {};
        for (int i = 0; i < _gridSize; ++i) {
            for (int j = 0; j < _gridSize; ++j) {
                // For each cell stored and its type is selected generate the box vertices (2 triangles)
				Cell cell = _map->get_cell(i, j);
                if (cell.get_type() == selectedType) {
                    GLfloat* boxVertices = generate_box(-1.0f + _cellSize / 2.0f + cell.get_x() * _cellSize, -1.0f + _cellSize / 2.0f + cell.get_y() * _cellSize);
                    for (int j = 0; j < 6 * 3; j++) {
                        gridBoxesVertices.push_back(boxVertices[j]);
                    }
                    delete[] boxVertices;
                }
            }
        }
        return gridBoxesVertices;
    }

	// Function to update the window with new information
    void update(VAO& boxVAOType1, VBO& boxVBOType1, VAO& boxVAOType2, VBO& boxVBOType2, VAO& boxVAOType3, VBO& boxVBOType3) {
       
		// Recall set snake to update the snake position from backend information
        _map->set_cells_empty();
        _snake->set_snake();
        _menu->check_lose_state();
        _map->set_food();

        // Generate and link VBOs to VAOs
        // Generate grid boxes vertices for each cell type 1
        gridBoxesVerticesType1 = {};
        gridBoxesVerticesType1 = generate_grid_boxes_vertices(1);
        boxVBOType1.Delete();
        boxVBOType1 = VBO(gridBoxesVerticesType1.data(), gridBoxesVerticesType1.size() * sizeof(GLfloat));
        boxVAOType1.Bind();
        boxVAOType1.LinkVBO(boxVBOType1, 0);
        boxVAOType1.Unbind();
        boxVBOType1.Unbind();
        // Generate grid boxes vertices for each cell type 2
        gridBoxesVerticesType2 = {};
        gridBoxesVerticesType2 = generate_grid_boxes_vertices(2);
        boxVBOType2.Delete();
        boxVBOType2 = VBO(gridBoxesVerticesType2.data(), gridBoxesVerticesType2.size() * sizeof(GLfloat));
        boxVAOType2.Bind();
        boxVAOType2.LinkVBO(boxVBOType2, 0);
        boxVAOType2.Unbind();
        boxVBOType2.Unbind();
        // Generate grid boxes vertices for each cell type 3
        gridBoxesVerticesType3 = {};
        gridBoxesVerticesType3 = generate_grid_boxes_vertices(3);
        boxVBOType3.Delete();
        boxVBOType3 = VBO(gridBoxesVerticesType3.data(), gridBoxesVerticesType3.size() * sizeof(GLfloat));
        boxVAOType3.Bind();
        boxVAOType3.LinkVBO(boxVBOType3, 0);
        boxVAOType3.Unbind();
        boxVBOType3.Unbind();
    }

    // EVENTS //

	// Funtion to manage key events callbacks
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS) {
            Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
            switch (key) {
            case GLFW_KEY_ENTER:
                std::cout << "Enter key pressed" << std::endl;
				win->_menu->callback_start(); // Start game
                break;
            case GLFW_KEY_ESCAPE:
                std::cout << "Scape key pressed" << std::endl;
                win->_menu->callback_pause(); // Pause game
                win->terminate(); // Terminate window
                break;
            case GLFW_KEY_SPACE:
                std::cout << "Space key pressed" << std::endl;
                win->_menu->callback_pause(); // Pause game
                break;
            case GLFW_KEY_W:
                std::cout << "W key pressed" << std::endl;
                win->_snake->callback_set_dir(0, 2); // Move up (y<0, x)
                break;
            case GLFW_KEY_S:
                std::cout << "S key pressed" << std::endl;
                win->_snake->callback_set_dir(0, 1); // Move down (y>0, x)
                break;
            case GLFW_KEY_A:
                std::cout << "A key pressed" << std::endl;
                win->_snake->callback_set_dir(2, 0); // Move left (y, x<0)
                break;
            case GLFW_KEY_D:
                std::cout << "D key pressed" << std::endl;
                win->_snake->callback_set_dir(1, 0); // Move right (y, x>0)
                break;
            case GLFW_KEY_UP:
                std::cout << "up key pressed" << std::endl;
                win->_snake->callback_set_dir(0, 2); // Move up (y<0, x)
                break;
            case GLFW_KEY_DOWN:
                std::cout << "down key pressed" << std::endl;
                win->_snake->callback_set_dir(0, 1); // Move down (y>0, x)
                break;
            case GLFW_KEY_LEFT:
                std::cout << "left key pressed" << std::endl;
                win->_snake->callback_set_dir(2, 0); // Move left (y, x<0)
                break;
            case GLFW_KEY_RIGHT:
                std::cout << "right key pressed" << std::endl;
                win->_snake->callback_set_dir(1, 0); // Move right (y, x>0)
                break;
			case GLFW_KEY_R:
				std::cout << "R key pressed" << std::endl;
				win->_menu->callback_pause(); // Pause game
				win->_menu->callback_reset(); // Reset game
				break;
            }
        }
    }

    // Function to manage window x close buttom callback 
    static void window_close_callback(GLFWwindow* window) {
        std::cout << "Window close button clicked" << std::endl;
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (win) {
            win->_menu->callback_pause(); //Pause game
            win->terminate(); // Call your cleanup function
        }
    }

};