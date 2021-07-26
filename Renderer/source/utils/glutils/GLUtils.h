#pragma once
#include <unordered_map>
#include<glad/glad.h>
#include <glm/glm.hpp>

namespace GLHelpers
{
   const std::unordered_map<unsigned int, unsigned int> GLTypeSizeLookUp
   {
      { GL_BYTE, sizeof(GLbyte) },
      { GL_UNSIGNED_BYTE, sizeof(GLubyte) },
      { GL_SHORT, sizeof(GLshort) },
      { GL_UNSIGNED_SHORT, sizeof(GLushort) },
      { GL_INT, sizeof(GLint) },
      { GL_UNSIGNED_INT, sizeof(GLuint) },
      { GL_HALF_FLOAT, sizeof(GLhalf) },
      { GL_FLOAT, sizeof(GLfloat) },
      { GL_DOUBLE, sizeof(GLdouble) }
   };
}


enum GLShaderType
{
   VERTEX_SHADER = GL_VERTEX_SHADER,
   GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
   FRAGMENT_SHADER = GL_FRAGMENT_SHADER
   //TESS_CONTROL_SHADER = GL_TESS_CONTROL_SHADER,       - Only available in GL >= 4.0
   //TESS_EVALUATION_SHADER = GL_TESS_EVALUATION_SHADER, - Only available in GL >= 4.0
   //COMPUTE_SHADER = GL_COMPUTE_SHADER                  - Only available in GL >= 4.3
};

enum GLPixelDataFormat
{
   NONE = 0,
   RED = GL_RED, 
   RG = GL_RG, 
   RGB = GL_RGB, 
   BGR = GL_BGR, 
   RGBA = GL_RGBA, 
   BRGA = GL_BGRA, 
   RED_INTEGER = GL_RED_INTEGER,
   RG_INTEGER = GL_RG_INTEGER,
   RGB_INTEGER = GL_RGB_INTEGER,
   BGR_INTEGER = GL_BGR_INTEGER,
   RGBA_INTEGER = GL_RGBA_INTEGER,
   BGRA_INTEGER = GL_BGRA_INTEGER,
   STENCIL_INDEX = GL_STENCIL_INDEX,
   DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
   DEPTH_STENCIL = GL_DEPTH_STENCIL
};

struct VertexAttribute
{
   VertexAttribute(unsigned int _size, unsigned int _type) : size(_size), type(_type) {}
   unsigned int size;
   unsigned int type;
};
