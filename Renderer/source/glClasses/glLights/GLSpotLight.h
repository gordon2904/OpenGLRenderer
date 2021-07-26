#pragma once
#include "GLPointLight.h"
class GLSpotLight :
    public GLPointLight
{
protected:
   glm::vec3 direction;
   float cutOff = glm::cos(glm::radians(12.5f));
   float outerCutOff = glm::cos(glm::radians(15.0f));
public:
   glm::vec3 getDirection() const;
   void setDirection(glm::vec3 value);
   float getCutOff() const;
   void setCutOff(float value);
   float getOuterCutOff() const;
   void setOuterCutOff(float value);
};

