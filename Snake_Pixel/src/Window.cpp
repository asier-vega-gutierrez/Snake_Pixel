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
#include "Cell.cpp"



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
    // Flag to indicate when the screen needs to be updated
    bool updateWindow = false;

public:

    // Constructor
    Window(int gridSize) :
        _gridSize(gridSize + 1),
        _cellSize(2.0f / (gridSize)),
        _cells(gridSize + 1, std::vector<Cell>(gridSize + 1)) {

        // Configure glfw
        glfwInit();
        // Using OpenGL 4.6
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
        // Using core profile 
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        // Set default map borders
        setUpCells();
    }

    int load() {
        // CONFIGURE BASE WINDOW
        // Create glfw window object
        window = glfwCreateWindow(width, height, title, NULL, NULL);
        if (window == NULL) {
            std::cout << "ERROR could not create GLFW window" << std::endl;
            terminate();
            return -1;
        }
        // Establish as current
        glfwMakeContextCurrent(window);
        // Set the key callback function
        glfwSetKeyCallback(window, key_callback);
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
        // Generates Vertex Array Object and binds it
        VAO boxVAOType1;
        boxVAOType1.Bind();
        // Generate grid filled box vertices
        std::vector<GLfloat> gridBoxesVerticesType1 = generate_grid_boxes_vertices(1);
        // Generates Vertex Buffer Object and links it to box vertices
        VBO boxVBOType1(gridBoxesVerticesType1.data(), gridBoxesVerticesType1.size() * sizeof(GLfloat));
        // Links VBO to VAO
        boxVAOType1.LinkVBO(boxVBOType1, 0);
        // Unbind all to prevent accidentally modifying them
        boxVAOType1.Unbind();
        boxVBOType1.Unbind();

        VAO boxVAOType2;
        boxVAOType2.Bind();
        // Generate grid filled box vertices
        std::vector<GLfloat> gridBoxesVerticesType2 = generate_grid_boxes_vertices(2);
        // Generates Vertex Buffer Object and links it to box vertices
        VBO boxVBOType2(gridBoxesVerticesType2.data(), gridBoxesVerticesType2.size() * sizeof(GLfloat));
        // Links VBO to VAO
        boxVAOType2.LinkVBO(boxVBOType2, 0);
        // Unbind all to prevent accidentally modifying them
        boxVAOType2.Unbind();
        boxVBOType2.Unbind();

        VAO boxVAOType3;
        boxVAOType3.Bind();
        // Generate grid filled box vertices
        std::vector<GLfloat> gridBoxesVerticesType3 = generate_grid_boxes_vertices(3);
        // Generates Vertex Buffer Object and links it to box vertices
        VBO boxVBOType3(gridBoxesVerticesType3.data(), gridBoxesVerticesType3.size() * sizeof(GLfloat));
        // Links VBO to VAO
        boxVAOType3.LinkVBO(boxVBOType3, 0);
        // Unbind all to prevent accidentally modifying them
        boxVAOType3.Unbind();
        boxVBOType3.Unbind();

        // Manage all events of the window while it is up
        while (!glfwWindowShouldClose(window)) {
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
            glfwSwapBuffers(window);
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
        glfwDestroyWindow(window);
        glfwTerminate();
        return 0;
    }

    // Function to set a box object on the grid vector
    int set_box_grid(int x, int y, int type) {
        Cell cell(x, y, type);
        _cells[cell.get_x()][cell.get_y()] = cell;
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
                Cell& cell = _cells[i][j];
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

    int setUpCells() {
        // Set all cells to type 0 (empty)
        for (int i = 0; i < _gridSize; ++i) {
            for (int j = 0; j < _gridSize; ++j) {
                Cell cell(i, j, 0);
                _cells[i][j] = cell;
            }
        }
        // Set borders of the grid to cell type 1
        for (int i = 0; i < _gridSize; ++i) {
            // Top border
            Cell cell_h_top(i, 0, 1);
            _cells[i][0] = cell_h_top;
            // Bottom border
            Cell cell_h_bottom(i, _gridSize - 2, 1);
            _cells[i][_gridSize - 1] = cell_h_bottom;
            // Left border
            Cell cell_v_left(0, i, 1);
            _cells[0][i] = cell_v_left;
            // Right border
            Cell cell_v_right(_gridSize - 2, i, 1);
            _cells[_gridSize - 2][i] = cell_v_right;
        }
        return 0;
    }

    // Static key callback function
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS) {
            switch (key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, true);
                break;
            case GLFW_KEY_W:
                std::cout << "W key pressed" << std::endl;
                break;
            case GLFW_KEY_S:
                std::cout << "S key pressed" << std::endl;
                break;
            case GLFW_KEY_A:
                std::cout << "A key pressed" << std::endl;
                break;
            case GLFW_KEY_D:
                std::cout << "D key pressed" << std::endl;
                break;
                // Add more cases for other keys as needed
            }
        }
    }


    int update() {
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (win) {
            win->updateWindow = true;
            std::cout << "Update need call to update Asier" << std::endl;
        }
        return 0;
    }
};