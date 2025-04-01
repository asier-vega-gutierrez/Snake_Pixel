#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "constants.h"
#include <vector>
#include "Box.h"


class Window{
	private:
		//Atributes
		GLFWwindow* window = {};
		int height = 800;
		int width = 800;
		const char* title = "actual screen";

		// Grid size is form 0 to n so add 1 for beter user experience
		int _gridSize = {};
		// Calculate the size of each cell in the grid
		const float _cellSize = {};
		// Al filled boxes on grid 
		std::vector<std::vector<Box>> _boxes = {};

	public:
		// Constructor
		Window(int gridSize): 
			_gridSize(gridSize + 1),
			_cellSize(2.0f / (gridSize)),
			_boxes(gridSize + 1, std::vector<Box>(gridSize + 1)) {

			// Configure glfw
			glfwInit();
			// Using OpenGL 4.6
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
			// Using core profile 
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		}

		int load() {
			//Create glfw window object
			window = glfwCreateWindow(width, height, title, NULL, NULL);
			if (window == NULL) {
				std::cout << "ERRO could not create GLFW window" << std::endl;
				terminate();
				return -1;
			}
			//Stablish as currect
			glfwMakeContextCurrent(window);
		
			//CONFIGURE BASE WINDOW
			//Load opengl
			gladLoadGL();
			//Define the opengl working zone
			glViewport(0, 0, width, height);
			//Generates Shader object using shaders defualt.vert and default.frag
			Shader shaderProgram(vertexShaderSource, fragmentShaderSource);

			//GRID
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
			VAO boxVAO;
			boxVAO.Bind();
			// Generate grid filled box vertices
			std::vector <GLfloat> gridBoxesVertices = {};
			for (int i = 0; i < 10; i++) { 
				GLfloat* boxVertices = generate_box(-1.0f + _cellSize / 2.0f + i * _cellSize, -1.0f + _cellSize / 2.0f + i * _cellSize);
				for (int j = 0; j < 6 * 3; j++) {
					gridBoxesVertices.push_back(boxVertices[j]);
				}
				delete[] boxVertices;
			}
			
			// Generates Vertex Buffer Object and links it to box vertices
			VBO boxVBO(gridBoxesVertices.data(), gridBoxesVertices.size() * sizeof(GLfloat));
			// Links VBO to VAO
			boxVAO.LinkVBO(boxVBO, 0);
			// Unbind all to prevent accidentally modifying them
			boxVAO.Unbind();
			boxVBO.Unbind();

			//Mange al events of the window while is up
			while (!glfwWindowShouldClose(window)) {
				//Specify the color of the background
				glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
				//Clean the back buffer and assign the new color to it
				glClear(GL_COLOR_BUFFER_BIT);
				//Tell OpenGL which Shader Program we want to use
				shaderProgram.Activate();
				// Bind the grid VAO and draw the grid
				gridVAO.Bind();
				glDrawArrays(GL_LINES, 0, _gridSize * 4);
				// Bind the triangle VAO and draw the triangle
				boxVAO.Bind();
				glDrawArrays(GL_TRIANGLES, 0, gridBoxesVertices.size() / 3);
				// Swap the back buffer with the front buffer
				glfwSwapBuffers(window);
				// Take care of all GLFW events
				glfwPollEvents();
			}

			// Delete all the objects we've created
			gridVAO.Delete();
			gridVBO.Delete();
			boxVAO.Delete();
			boxVAO.Delete();
			shaderProgram.Delete();
			
			delete[] gridVertices;

			return 0;
		}

		int terminate() {
			glfwDestroyWindow(window);
			glfwTerminate();
			return 0;
		}

		int set_box_grid(int x, int y, int type) {
			Box box(x, y ,type);
			_boxes[box._x][box._y] = box;
			std::cout << "Box setted at x: " << _boxes[x][y]._x << " y: " << _boxes[x][y]._y << " type: " << _boxes[x][y]._type << std::endl;
			return 0;
		}

	private:
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


		
		

};

