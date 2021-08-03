#pragma once
#include "../GLEntity.h"

class GLLight : public GLEntity
{
protected:
   glm::vec3 ambientColour = { 1.0f, 1.0f, 1.0f };
   glm::vec3 diffuseColour = { 1.0f, 1.0f, 1.0f };
   glm::vec3 specularColour = { 1.0f, 1.0f, 1.0f };
public:
   GLLight(void* data, unsigned int _dataLength, std::vector<VertexAttribute>& vertexAttributes);
   GLLight(void* data, unsigned int _dataLength, std::vector<VertexAttribute>& vertexAttributes, void* elements, unsigned int _elementsLength);
   glm::vec3 getDiffuseColour() const;
   void setDiffuseColour(const glm::vec3 colour);
   glm::vec3 getAmbientColour() const;
   void setAmbientColour(const glm::vec3 colour);
   glm::vec3 getSpecularColour() const;
   void setSpecularColour(const glm::vec3 colour);
   int virtual render(RenderInputs& input);
};