#pragma once
#include "glDrawables/GLDrawable.h"

class GLMultiEntity : public GLDrawable
{
protected:
   std::vector<glm::mat4> models; 
   std::function<void(glm::mat4&, std::shared_ptr<Material>, const float&, unsigned int)> mUpdateLambda;
public:
   GLMultiEntity(void* data, unsigned int _dataLength, std::vector<VertexAttribute>& vertexAttributes);
   GLMultiEntity(void* data, unsigned int _dataLength, std::vector<VertexAttribute>& vertexAttributes, void* elements, unsigned int _elementsLength);
   void setModelCount(unsigned int count);
   int virtual Render(const float& time, std::shared_ptr<Material> overrideMaterial = nullptr);
   void setModelMat(const glm::mat4 mModel, unsigned int index);
   glm::mat4* getModelMat(unsigned int index);
   void setUpdateLambda(std::function<void(glm::mat4&, std::shared_ptr<Material>, const float&, unsigned int)> updateLambda);
};