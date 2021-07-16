#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 uColor;
  
uniform float offset; // we set this variable in the OpenGL code.
out vec4 vertexColor; // specify a color output to the fragment shader

void main()
{
    gl_Position = vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
	gl_Position.x += offset;
    vertexColor = vec4(uColor, 1.0);//vec4(gl_Position.xyz, 1.0); // set the output variable to a dark-red color
}