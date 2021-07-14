#pragma once
#include "GLEntity.h"
class GLEntityPolygon :
    public GLEntity
{
private:
   float* mVertices;
   unsigned int mVerticesLength;
   unsigned int* mIndices;
   unsigned int mIndicesLength;
   unsigned int ebo;
public:
   GLEntityPolygon(float* vertices, unsigned int verticesLength, unsigned int* indices, unsigned int indicesLength); 
   ~GLEntityPolygon();
   int Render(const unsigned int& shaderProgram);
};