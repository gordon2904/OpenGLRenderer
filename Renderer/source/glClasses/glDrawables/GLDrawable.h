#pragma once
#include <functional>
#include <memory>
#include <unordered_map>
#include <array>
#include "../../utils/glutils/GLUtils.h"
#include "../Material.h"

struct RenderInputs
{
   std::shared_ptr<Material> overrideMaterial = nullptr;
   float time;
   glm::mat4 view;
   glm::mat4 projection;
};

class GLDrawable
{
   //declared properties and methods
protected:
   //size in bytes
   unsigned int vboDataSize;
   //length in strides
   unsigned int vboDataLength;
   unsigned int elementsLength;
   unsigned int vao;
   unsigned int vbo;
   unsigned int ebo;
   std::shared_ptr<Material> mMaterial;
   bool mVisible; 
public:
   GLDrawable(void* data, unsigned int _dataLength, std::vector<VertexAttribute>& vertexAttributes);
   GLDrawable(void* data, unsigned int _dataLength, std::vector<VertexAttribute>& vertexAttributes, void* elements, unsigned int _elementsLength);
   virtual ~GLDrawable();
   int virtual render(RenderInputs& input) = 0;

   //defined methods
protected:
   //extern const std::unordered_map<unsigned int, unsigned int> GLTypeSizeLookUp;
   unsigned int calculateVBOStride(const std::vector<VertexAttribute>& vertexAttributes)
   {
      unsigned int stride = 0;
      for(VertexAttribute vertexAttribute : vertexAttributes)
      {
         stride += GLHelpers::GLTypeSizeLookUp.at(vertexAttribute.type) * vertexAttribute.size;
      }
      return stride;
   }
   std::shared_ptr<Material> useMaterial(const std::shared_ptr<Material> overrideMaterial)
   {
      if(overrideMaterial != nullptr)
      {
         overrideMaterial->use();
         return overrideMaterial;
      }
      else if(mMaterial != nullptr)
      {
         mMaterial->use();
         return mMaterial;
      }
      return nullptr;
   }
   void draw()
   {
      if(ebo > 0)
      {
         glDrawElements(GL_TRIANGLES, elementsLength, GL_UNSIGNED_INT, 0);
      }
      else
      {
         glDrawArrays(GL_TRIANGLES, 0, vboDataLength);
      }
   }
public:
   void setVisible(const bool& visible)
   {
      mVisible = visible;
   }
   void setMaterial(std::shared_ptr<Material> material)
   {
      mMaterial = material;
   }
};