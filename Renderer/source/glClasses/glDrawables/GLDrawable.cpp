#include "GLDrawable.h"
#include <glad/glad.h>
#include "../../utils/logger/Logger.h"

GLDrawable::GLDrawable(void* data, unsigned int dataSize, std::vector<VertexAttribute>& vertexAttributes) : GLDrawable(data, dataSize, vertexAttributes, nullptr, 0) {}

GLDrawable::GLDrawable(void* data, unsigned int dataSize, std::vector<VertexAttribute>& vertexAttributes, void* elements, unsigned int _elementsLength) 
   : material(nullptr), ebo(0), mVisible(true), vboDataSize(dataSize), elementsLength(_elementsLength)
{
   glGenVertexArrays(1, &vao);
   glGenBuffers(1, &vbo);
   if(elements != nullptr)
   {
      glGenBuffers(1, &ebo);
   }
   const unsigned int stride = calculateVBOStride(vertexAttributes);
   vboDataLength = vboDataSize / stride;

   glBindVertexArray(vao);

   if(elements != nullptr)
   {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * elementsLength, elements, GL_STATIC_DRAW);
   }

   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, vboDataSize, data, GL_STATIC_DRAW);

   unsigned int trackedOffset = 0;
   for(unsigned int i = 0; i < vertexAttributes.size(); ++i)
   {
      const VertexAttribute vertexAttribute = vertexAttributes[i];
      GLintptr offset = trackedOffset;
      glVertexAttribPointer(i, vertexAttribute.size, vertexAttribute.type, GL_FALSE, stride, (void*)offset);
      glEnableVertexAttribArray(i); 
      trackedOffset += GLHelpers::GLTypeSizeLookUp.at(vertexAttribute.type) * vertexAttribute.size;
   }
}

GLDrawable::~GLDrawable()
{
   LOG_INFO("deleting GLDrawable\nvao: {0}\nvbo: {1}\nebo: {2}", vao, vbo, ebo);
   glDeleteVertexArrays(1, &vao);
   glDeleteBuffers(1, &vbo);
   glDeleteBuffers(1, &ebo);
}