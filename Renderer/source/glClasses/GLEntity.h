#pragma once
#include "glDrawables/GLDrawable.h"

class GLEntity : public GLDrawable
{
protected:
   glm::mat4 model; 
   std::function<void(glm::mat4&, std::shared_ptr<Material>, const float&)> updateLambda;
   void updateModelViewProjection(std::shared_ptr<Material> material, glm::mat4& view, glm::mat4& projection);
public:
   GLEntity(void* data, unsigned int _dataLength, std::vector<VertexAttribute>& vertexAttributes);
   GLEntity(void* data, unsigned int _dataLength, std::vector<VertexAttribute>& vertexAttributes, void* elements, unsigned int _elementsLength);
   int virtual render(RenderInputs& input);
   void setModelMat(const glm::mat4 mModel);
   glm::mat4* getModelMat();
   void setUpdateLambda(std::function<void(glm::mat4&, std::shared_ptr<Material>, const float&)> updateLambda);
   glm::vec3 getPosition() const;
};