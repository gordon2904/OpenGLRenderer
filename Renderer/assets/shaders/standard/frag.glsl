#version 330 core
out vec4 FragColor;
  
in vec4 vertexColor;
uniform vec4 ourColor; // we set this variable in the OpenGL code.

void main()
{
    FragColor = vertexColor;
}   