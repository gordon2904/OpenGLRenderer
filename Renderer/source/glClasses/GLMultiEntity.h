#pragma once
#include "glDrawables/GLDrawable.h"

//there is a much better way of doing this that only requires one call gldraw call (instancing), this needs revisiting
class GLMultiEntity : public GLDrawable
{
protected:
   std::vector<glm::mat4> models; 
   std::function<void(glm::mat4&, const Material*, const float&, unsigned int)> updateLambda;
public:
   GLMultiEntity(void* data, unsigned int _dataLength, std::vector<VertexAttribute>& vertexAttributes);
   GLMultiEntity(void* data, unsigned int _dataLength, std::vector<VertexAttribute>& vertexAttributes, void* elements, unsigned int _elementsLength);
   void setModelCount(unsigned int count);
   int render(RenderInputs& input);
   void setModelMat(const glm::mat4 mModel, unsigned int index);
   glm::mat4* getModelMat(unsigned int index);
   void updateModelViewProjection(const Material* material, glm::mat4& view, glm::mat4& projection);
   void setUpdateLambda(std::function<void(glm::mat4&, const Material*, const float&, unsigned int)> updateLambda);
};