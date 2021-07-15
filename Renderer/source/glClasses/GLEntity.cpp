#include "GLEntity.h"
#include <glad/glad.h>
#include "../utils/glutils/GLUtils.h"
#include "../utils/logger/Logger.h"

GLEntity::GLEntity(void* data, unsigned int dataLength, std::vector<VertexAttribute>& vertexAttributes) : GLEntity(data, dataLength, vertexAttributes, nullptr, 0) {}

GLEntity::GLEntity(void* data, unsigned int _dataLength, std::vector<VertexAttribute>& vertexAttributes, void* elements, unsigned int _elementsLength) : ebo(0), mVisible(true), mShaderProgram(0), mUpdateLambda([] () {}), vboDataLength(_dataLength), elementsLength(_elementsLength)
{
   glGenVertexArrays(1, &vao);
   glGenBuffers(1, &vbo);
   if(elements != nullptr)
   {
      glGenBuffers(1, &ebo);
   }
   const unsigned int stride = CalculateVBOStride(vertexAttributes);

   LOG_INFO("ebo value: {0}", ebo);

   glBindVertexArray(vao);

   if(elements != nullptr)
   {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * elementsLength, elements, GL_STATIC_DRAW);
   }

   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, stride * vboDataLength, data, GL_STATIC_DRAW);

   unsigned int trackedOffset = 0;
   for(unsigned int i = 0; i < vertexAttributes.size(); ++i)
   {
      const VertexAttribute vertexAttribute = vertexAttributes[i];
      GLintptr offset = trackedOffset;
      glVertexAttribPointer(i, vertexAttribute.size, vertexAttribute.type, GL_FALSE, stride, (void*)offset);
      glEnableVertexAttribArray(i);
      trackedOffset += typeSizeLookUp.at(vertexAttribute.type) * vertexAttribute.size;
   }
}

const std::unordered_map<unsigned int, unsigned int> GLEntity::typeSizeLookUp{
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

unsigned int GLEntity::CalculateVBOStride(const std::vector<VertexAttribute>& vertexAttributes)
{
   unsigned int stride = 0;
   for(VertexAttribute vertexAttribute : vertexAttributes)
   {
      stride += typeSizeLookUp.at(vertexAttribute.type) * vertexAttribute.size;
   }
   return stride;
}

GLEntity::~GLEntity()
{
   LOG_INFO("deleting\nvao: {0}\nvbo: {1}", vao, vbo);
   glDeleteVertexArrays(1, &vao);
   glDeleteBuffers(1, &vbo);
}

int GLEntity::Render(const unsigned int shaderProgram)
{
   if(!mVisible)
   {
      return 0;
   }
   mUpdateLambda();
   if(shaderProgram > 0)
   {
      glUseProgram(shaderProgram);
   }
   else if(mShaderProgram > 0)
   {
      glUseProgram(mShaderProgram);
   }
   else
   {
      return 0;
   }
   glBindVertexArray(vao);
   Draw();
   return 1;
}


void GLEntity::Draw()
{
   if(ebo > 0)
   {
      glDrawElements(GL_TRIANGLES, elementsLength, GL_UNSIGNED_INT, 0);
   }
   else
   {
      glDrawArrays(GL_TRIANGLES, 0, vboDataLength);
   }
}

void GLEntity::setVisible(const bool& visible)
{
   mVisible = visible;
}


void GLEntity::setShaderProgram(unsigned int shaderProgram)
{
   mShaderProgram = shaderProgram;
}

unsigned int GLEntity::getShaderProgram()
{
   return mShaderProgram;
}


void GLEntity::setUpdateLambda(std::function<void()> updateLambda)
{
   mUpdateLambda = updateLambda;
}