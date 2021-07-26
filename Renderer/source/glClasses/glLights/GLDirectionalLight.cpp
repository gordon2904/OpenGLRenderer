#include "GLDirectionalLight.h"
glm::vec3 GLDirectionalLight::getDirection() const
{
   return direction;
}
void GLDirectionalLight::setDirection(glm::vec3 value)
{
   direction = value;
}