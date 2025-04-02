#include "shader.h"


// Constructor that build the Shader Program from 2 different shaders
Shader::Shader(const char* vertexFile, const char* fragmentFile) {
	//Create Vertex Shader Object and get its reference
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	//Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertex_shader, 1, &vertexFile, NULL);
	//Compile the Vertex Shader into machine code
	glCompileShader(vertex_shader);

	//Create Fragment Shader Object and get its reference
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	//Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragment_shader, 1, &fragmentFile, NULL);
	//Compile the Vertex Shader into machine code
	glCompileShader(fragment_shader);

	//Create Shader Program Object and get its reference
	ID = glCreateProgram();
	//Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(ID, vertex_shader);
	glAttachShader(ID, fragment_shader);
	//Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(ID);

	//Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

}

// Activates the Shader Program
void Shader::Activate() {
	glUseProgram(ID);
}

// Deletes the Shader Program
void Shader::Delete() {
	glDeleteProgram(ID);
}

// Set the color of the shader
void Shader::setColor(float r, float g, float b) {
	glUniform3f(glGetUniformLocation(ID, "uColor"), r, g, b);
}