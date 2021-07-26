#include "GLPointLight.h"


GLPointLight::GLPointLight(void* data, unsigned int dataSize, std::vector<VertexAttribute>& vertexAttributes)
   : GLLight(data, dataSize, vertexAttributes, nullptr, 0)
{}

GLPointLight::GLPointLight(void* data, unsigned int dataSize, std::vector<VertexAttribute>& vertexAttributes, void* elements, unsigned int _elementsLength)
   : GLLight(data, dataSize, vertexAttributes, elements, _elementsLength)
{}

float GLPointLight::getConstant() const
{
   return constant;
}
void GLPointLight::setConstant(float value)
{
   constant = value;
}
float GLPointLight::getLinear() const
{
   return linear;
}
void GLPointLight::setLinear(float value)
{
   linear = value;
}
float GLPointLight::getQuadratic() const
{
   return quadratic;
}
void GLPointLight::setQuadratic(float value)
{
   quadratic = value;
}