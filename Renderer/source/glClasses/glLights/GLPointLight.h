#pragma once
#include "GLLight.h"
class GLPointLight :
    public GLLight
{
protected:
   float constant = 1.0f;
   float linear = 0.09f;
   float quadratic = 0.032f;
public:
   GLPointLight(void* data, unsigned int _dataLength, std::vector<VertexAttribute>& vertexAttributes);
   GLPointLight(void* data, unsigned int _dataLength, std::vector<VertexAttribute>& vertexAttributes, void* elements, unsigned int _elementsLength);
   float getConstant() const;
   void setConstant(float value);
   float getLinear() const;
   void setLinear(float value);
   float getQuadratic() const;
   void setQuadratic(float value);
};

