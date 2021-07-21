#pragma once
#include "glDrawables/GLDrawable.h"

class GLEntity : public GLDrawable
{
protected:
   glm::mat4 model; 
   std::function<void(glm::mat4&, std::shared_ptr<Material>, const float&)> mUpdateLambda;
public:
   GLEntity(void* data, unsigned int _dataLength, std::vector<VertexAttribute>& vertexAttributes);
   GLEntity(void* data, unsigned int _dataLength, std::vector<VertexAttribute>& vertexAttributes, void* elements, unsigned int _elementsLength);
   int virtual Render(const float& time, std::shared_ptr<Material> overrideMaterial = nullptr);
   void setModelMat(const glm::mat4 mModel);
   glm::mat4* getModelMat();
   void setUpdateLambda(std::function<void(glm::mat4&, std::shared_ptr<Material>, const float&)> updateLambda);
};