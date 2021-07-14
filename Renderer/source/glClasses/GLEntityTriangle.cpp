#include "GLEntityTriangle.h"
#include <glad/glad.h>

GLEntityTriangle::GLEntityTriangle(float* vertices, unsigned int verticesLength) : mVertices(vertices), mVerticesLength(verticesLength)
{
   glBindVertexArray(vao);

   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesLength, vertices, GL_STATIC_DRAW);

   //inform the buffer object of its data type and size, as this will be a vec3 we use sizeof(float) * 3 to get the bytes in each stride
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
}

int GLEntityTriangle::Render(const unsigned int& shaderProgram)
{
   if(!mVisible)
   {
      return 0;
   }
   GLEntity::Render(shaderProgram);
   glDrawArrays(GL_TRIANGLES, 0, mVerticesLength / 3);
   return 1;
}