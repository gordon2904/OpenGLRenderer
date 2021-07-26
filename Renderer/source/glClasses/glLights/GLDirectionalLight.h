#pragma once
#include "GLLight.h"
class GLDirectionalLight :
    public GLLight
{
protected: 
   glm::vec3 direction;
public:
   glm::vec3 getDirection() const;
   void setDirection(glm::vec3 value);
};

