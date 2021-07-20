#include "GLEntity.h"
#include <glad/glad.h>
#include "../utils/glutils/GLUtils.h"
#include "../utils/logger/Logger.h"

//static members
const std::function<void(std::shared_ptr<Material>, const float&)> GLEntity::defaultUpdateLambda = [] (std::shared_ptr<Material>, const float&) {};

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

GLEntity::GLEntity(void* data, unsigned int dataSize, std::vector<VertexAttribute>& vertexAttributes) : GLEntity(data, dataSize, vertexAttributes, nullptr, 0) {}

GLEntity::GLEntity(void* data, unsigned int dataSize, std::vector<VertexAttribute>& vertexAttributes, void* elements, unsigned int _elementsLength) : mMaterial(nullptr), ebo(0), mVisible(true), mUpdateLambda(defaultUpdateLambda), vboDataSize(dataSize), elementsLength(_elementsLength)
{
   glGenVertexArrays(1, &vao);
   glGenBuffers(1, &vbo);
   if(elements != nullptr)
   {
      glGenBuffers(1, &ebo);
   }
   const unsigned int stride = CalculateVBOStride(vertexAttributes);
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
      trackedOffset += typeSizeLookUp.at(vertexAttribute.type) * vertexAttribute.size;
   }
}

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
   LOG_INFO("deleting GLEntity\nvao: {0}\nvbo: {1}", vao, vbo);
   glDeleteVertexArrays(1, &vao);
   glDeleteBuffers(1, &vbo);
}

int GLEntity::useMaterial(const float &time, std::shared_ptr<Material> overrideMaterial)
{
   if(overrideMaterial != nullptr)
   {
      overrideMaterial->use();
      mUpdateLambda(overrideMaterial, time);
   }
   else if(mMaterial != nullptr)
   {
      mMaterial->use();
      mUpdateLambda(mMaterial, time);
   }
   else
   {
      return 0; 
   }
   return 1;
}

int GLEntity::Render(const float &time, std::shared_ptr<Material> overrideMaterial)
{
   if(!mVisible)
   {
      return 0;
   }
   if(!useMaterial(time, overrideMaterial))
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

void GLEntity::setMaterial(std::shared_ptr<Material> material)
{
   mMaterial = material;
}


void GLEntity::setUpdateLambda(std::function<void(std::shared_ptr<Material>, const float&)> updateLambda)
{
   mUpdateLambda = updateLambda;
}