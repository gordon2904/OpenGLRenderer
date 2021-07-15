#pragma once
#include<glad/glad.h>

enum GLShaderType
{
   VERTEX_SHADER = GL_VERTEX_SHADER,
   GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
   FRAGMENT_SHADER = GL_FRAGMENT_SHADER
   //TESS_CONTROL_SHADER = GL_TESS_CONTROL_SHADER,       - Only available in GL >= 4.0
   //TESS_EVALUATION_SHADER = GL_TESS_EVALUATION_SHADER, - Only available in GL >= 4.0
   //COMPUTE_SHADER = GL_COMPUTE_SHADER                  - Only available in GL >= 4.3
};

struct VertexAttribute
{
   VertexAttribute(unsigned int _size, unsigned int _type) : size(_size), type(_type) {}
   unsigned int size;
   unsigned int type;
};
