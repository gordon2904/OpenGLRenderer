#include "GLEntity.h"
#include <glad/glad.h>
#include "../utils/glutils/GLUtils.h"
#include "../utils/logger/Logger.h"

GLEntity::GLEntity(void* data, unsigned int dataSize, std::vector<VertexAttribute>& vertexAttributes)
   : GLEntity(data, dataSize, vertexAttributes, nullptr, 0) {}

GLEntity::GLEntity(void* data, unsigned int dataSize, std::vector<VertexAttribute>& vertexAttributes, void* elements, unsigned int _elementsLength)
   : GLDrawable(data, dataSize, vertexAttributes, elements, _elementsLength), 
     model(glm::mat4(1.0f)), mUpdateLambda([] (glm::mat4&, std::shared_ptr<Material>, const float&) {}) {}

int GLEntity::Render(const float &time, std::shared_ptr<Material> overrideMaterial)
{
   if(!mVisible)
   {
      return 0;
   }
   std::shared_ptr<Material> material = useMaterial(overrideMaterial);
   if(material == nullptr)
   {
      return 0;
   }
   mUpdateLambda(model, material, time);
   glBindVertexArray(vao);
   Draw();
   return 1;
}

void GLEntity::setModelMat(const glm::mat4 mModel)
{
   model = mModel;
}
glm::mat4* GLEntity::getModelMat()
{
   return &model;
}


void GLEntity::setUpdateLambda(std::function<void(glm::mat4&, std::shared_ptr<Material>, const float&)> updateLambda)
{
   mUpdateLambda = updateLambda;
}