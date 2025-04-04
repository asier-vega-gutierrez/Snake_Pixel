#pragma once

// Define the shader source code
const char* vertexShaderSource = R"(
#version 460 core
layout(location = 0) in vec3 aPos;
void main()
{
    gl_Position = vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 460 core
out vec4 FragColor;
uniform vec3 uColor;
void main()
{
    FragColor = vec4(uColor, 1.0);
}
)";