#pragma once
#include "GLEntity.h"
class GLEntityTriangle :
    public GLEntity
{
private:
   float *mVertices;
   unsigned int mVerticesLength;
public:
   GLEntityTriangle(float *vertices, unsigned int verticesLength);
   int Render(const unsigned int& shaderProgram);
};

