#include "GLMultiEntity.h"
#include <glad/glad.h>
#include "../utils/glutils/GLUtils.h"
#include "../utils/logger/Logger.h"

void updateViewProjection(const Material* material, glm::mat4& view, glm::mat4& projection);

GLMultiEntity::GLMultiEntity(void* data, unsigned int dataSize, std::vector<VertexAttribute>& vertexAttributes)
   : GLMultiEntity(data, dataSize, vertexAttributes, nullptr, 0) {}

GLMultiEntity::GLMultiEntity(void* data, unsigned int dataSize, std::vector<VertexAttribute>& vertexAttributes, void* elements, unsigned int _elementsLength)
   : GLDrawable(data, dataSize, vertexAttributes, elements, _elementsLength),
   models(std::vector<glm::mat4>()), updateLambda([] (glm::mat4&, const Material*, const float&, unsigned int) {})
{}

int GLMultiEntity::render(RenderInputs &input)
{
   if(!mVisible)
   {
      return 0;
   }
   const Material* material = useMaterial(input.overrideMaterial);
   if(material == nullptr)
   {
      return 0;
   }
   glBindVertexArray(vao); 
   updateViewProjection(material, input.view, input.projection);
   for(unsigned int i = 0; i < models.size(); ++i)
   {
      glm::mat4 model = models[i];
      updateLambda(model, material, input.time, i);
      material->setMat4("model", model);
      draw();
   }
   return 1;
}

void updateViewProjection(const Material* material, glm::mat4& view, glm::mat4& projection)
{
   material->setMat4("view", view);
   material->setMat4("projection", projection);
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

void GLMultiEntity::setUpdateLambda(std::function<void(glm::mat4&, const Material*, const float&, unsigned int)> updateLambda)
{
   this->updateLambda = updateLambda;
}