#include "GLSpotLight.h"

glm::vec3 GLSpotLight::getDirection() const
{
   return direction;
}
void GLSpotLight::setDirection(glm::vec3 value)
{
   direction = value;
}
float GLSpotLight::getCutOff() const
{
   return cutOff;
}
void GLSpotLight::setCutOff(float value)
{
   cutOff = value;
}
float GLSpotLight::getOuterCutOff() const
{
   return outerCutOff;
}
void GLSpotLight::setOuterCutOff(float value)
{
   outerCutOff = value;
}