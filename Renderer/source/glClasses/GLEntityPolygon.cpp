#include "GLEntityPolygon.h"
#include <glad/glad.h>
#include "../utils/logger/Logger.h"


GLEntityPolygon::~GLEntityPolygon()
{
   LOG_INFO("deleting\nebo: {0}", ebo);
   glDeleteBuffers(1, &ebo);
}

GLEntityPolygon::GLEntityPolygon(float* vertices, unsigned int verticesLength, unsigned int* indices, unsigned int indicesLength) 
   : mVertices(vertices), mVerticesLength(verticesLength), mIndices(indices), mIndicesLength(indicesLength)
{
   glGenBuffers(1, &ebo);
   glBindVertexArray(vao);

   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesLength, vertices, GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesLength, indices, GL_STATIC_DRAW);

   //inform the buffer object of its data type and size, as this will be a vec3 we use sizeof(float) * 3 to get the bytes in each stride
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
}

int GLEntityPolygon::Render(const unsigned int& shaderProgram)
{
   if(!mVisible)
   {
      return 0;
   }
   GLEntity::Render(shaderProgram);
   glDrawElements(GL_TRIANGLES, mIndicesLength, GL_UNSIGNED_INT, 0);
   return 1;
}