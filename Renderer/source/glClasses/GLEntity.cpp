#include "GLEntity.h"
#include <glad/glad.h>
#include "../utils/glutils/GLUtils.h"
#include "../utils/logger/Logger.h"

GLEntity::GLEntity(void* data, unsigned int dataSize, std::vector<VertexAttribute>& vertexAttributes) : GLEntity(data, dataSize, vertexAttributes, nullptr, 0) {}

GLEntity::GLEntity(void* data, unsigned int dataSize, std::vector<VertexAttribute>& vertexAttributes, void* elements, unsigned int _elementsLength) : shader(nullptr), ebo(0), mVisible(true), mUpdateLambda([] () {}), vboDataSize(dataSize), elementsLength(_elementsLength)
{
   glGenVertexArrays(1, &vao);
   glGenBuffers(1, &vbo);
   if(elements != nullptr)
   {
      glGenBuffers(1, &ebo);
   }
   const unsigned int stride = CalculateVBOStride(vertexAttributes);
   vboDataLength = vboDataSize / stride;

   LOG_INFO("ebo value: {0}", ebo);

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
      LOG_INFO("glVertexAttribPointer({0}, {1}, {2}, GL_FALSE, {3}, (void*){4}", i, vertexAttribute.size, vertexAttribute.type, stride, offset);
      glVertexAttribPointer(i, vertexAttribute.size, vertexAttribute.type, GL_FALSE, stride, (void*)offset);
      glEnableVertexAttribArray(i);
      trackedOffset += typeSizeLookUp.at(vertexAttribute.type) * vertexAttribute.size;
   }
   LOG_INFO("vbo Data Length: {0}", vboDataLength);
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

int GLEntity::useShader(std::shared_ptr<Shader> overrideShader)
{
   if(overrideShader != nullptr)
   {
      overrideShader->use();
   }
   else if(shader != nullptr)
   {
      shader->use();
   }
   else
   {
      return 0;
   }
   return 1;
}

int GLEntity::Render(std::shared_ptr<Shader> overrideShader)
{
   if(!mVisible)
   {
      return 0;
   }
   if(texture != nullptr)
   {
      texture->use();
   }
   if(!useShader(overrideShader))
   {
      return 0;
   }
   mUpdateLambda();
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

void GLEntity::setShaderProgram(std::shared_ptr<Shader> _shader)
{
   shader = _shader;
}

std::shared_ptr<Shader> GLEntity::getShaderProgram()
{
   return shader;
}

void GLEntity::setTexture(std::shared_ptr<Texture> _texture)
{
   texture = _texture;
}

std::shared_ptr<Texture> GLEntity::getTexture()
{
   return texture;
}

void GLEntity::setUpdateLambda(std::function<void()> updateLambda)
{
   mUpdateLambda = updateLambda;
}