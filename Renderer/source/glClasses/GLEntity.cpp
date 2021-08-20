#include "GLEntity.h"
#include <glad/glad.h>
#include "../utils/glutils/GLUtils.h"
#include "../utils/logger/Logger.h"

GLEntity::GLEntity(void* data, unsigned int dataSize, std::vector<VertexAttribute>& vertexAttributes)
   : GLEntity(data, dataSize, vertexAttributes, nullptr, 0) {}

GLEntity::GLEntity(void* data, unsigned int dataSize, std::vector<VertexAttribute>& vertexAttributes, void* elements, unsigned int _elementsLength)
   : GLDrawable(data, dataSize, vertexAttributes, elements, _elementsLength), 
     model(glm::mat4(1.0f)), updateLambda([] (glm::mat4&, const Material*, const float&) {}) {}

int GLEntity::render(RenderInputs& input)
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
   updateLambda(model, material, input.time);
   updateModelViewProjection(material, input.view, input.projection);
   glBindVertexArray(vao);
   draw();
   return 1;
}

void GLEntity::updateModelViewProjection(const Material* material, glm::mat4& view, glm::mat4& projection)
{
   material->setMat4("model", model);
   material->setMat4("view", view);
   material->setMat4("projection", projection);
}

void GLEntity::setModelMat(const glm::mat4 mModel)
{
   model = mModel;
}
glm::mat4* GLEntity::getModelMat()
{
   return &model;
}


void GLEntity::setUpdateLambda(std::function<void(glm::mat4&, const Material*, const float&)> updateLambda)
{
   this->updateLambda = updateLambda;
}

glm::vec3 GLEntity::getPosition() const
{
   return glm::vec3(model[3]);
}