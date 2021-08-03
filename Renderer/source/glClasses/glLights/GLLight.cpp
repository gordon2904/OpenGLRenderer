#include "GLLight.h"

GLLight::GLLight(void* data, unsigned int dataSize, std::vector<VertexAttribute>& vertexAttributes)
   : GLEntity(data, dataSize, vertexAttributes, nullptr, 0)
{}

GLLight::GLLight(void* data, unsigned int dataSize, std::vector<VertexAttribute>& vertexAttributes, void* elements, unsigned int _elementsLength)
   : GLEntity(data, dataSize, vertexAttributes, elements, _elementsLength)
{}

glm::vec3 GLLight::getDiffuseColour() const
{
   return diffuseColour;
}

void GLLight::setDiffuseColour(const glm::vec3 colour)
{
   diffuseColour = colour;
}

glm::vec3 GLLight::getAmbientColour() const
{
   return ambientColour;
}

void GLLight::setAmbientColour(const glm::vec3 colour)
{
   ambientColour = colour;
}

glm::vec3 GLLight::getSpecularColour() const
{
   return specularColour;
}

void GLLight::setSpecularColour(const glm::vec3 colour)
{
   specularColour = colour;
}

//lightColour

int GLLight::render(RenderInputs& input)
{
   if(!mVisible)
   {
      return 0;
   }
   std::shared_ptr<Material> material = useMaterial(input.overrideMaterial);
   if(material == nullptr)
   {
      return 0;
   }
   updateModelViewProjection(material, input.view, input.projection);
   updateLambda(model, material, input.time);
   material->setVec3("lightColour", diffuseColour);
   glBindVertexArray(vao);
   draw();
   return 1;
}