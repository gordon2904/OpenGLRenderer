#include "GLMultiEntity.h"
#include <glad/glad.h>
#include "../utils/glutils/GLUtils.h"
#include "../utils/logger/Logger.h"

GLMultiEntity::GLMultiEntity(void* data, unsigned int dataSize, std::vector<VertexAttribute>& vertexAttributes)
   : GLMultiEntity(data, dataSize, vertexAttributes, nullptr, 0) {}

GLMultiEntity::GLMultiEntity(void* data, unsigned int dataSize, std::vector<VertexAttribute>& vertexAttributes, void* elements, unsigned int _elementsLength)
   : GLDrawable(data, dataSize, vertexAttributes, elements, _elementsLength),
   models(std::vector<glm::mat4>()), mUpdateLambda([] (glm::mat4&, std::shared_ptr<Material>, const float&, unsigned int) {})
{}

int GLMultiEntity::Render(const float &time, std::shared_ptr<Material> overrideMaterial)
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
   glBindVertexArray(vao);
   for(unsigned int i = 0; i < models.size(); ++i)
   {
      glm::mat4 model = models[i];
      mUpdateLambda(model, material, time, i);
      Draw();
   }
   return 1;
}

void GLMultiEntity::setModelMat(const glm::mat4 mModel, unsigned int index)
{
   models[index] = mModel;
}
glm::mat4* GLMultiEntity::getModelMat(unsigned int index)
{
   return &models[index];
}

void GLMultiEntity::setModelCount(unsigned int count)
{
   const unsigned int originalSize = models.size();
   models.resize(count);
   for(int i = originalSize - 1; i < models.size(); ++i)
   {
      models[i] = glm::mat4(1);
   }
}

void GLMultiEntity::setUpdateLambda(std::function<void(glm::mat4&, std::shared_ptr<Material>, const float&, unsigned int)> updateLambda)
{
   mUpdateLambda = updateLambda;
}