#pragma once
#include <functional>
#include <memory>
#include <unordered_map>
#include <array>
#include "../utils/glutils/GLUtils.h"
#include "Material.h"

class GLEntity
{
protected:
   int useMaterial(std::shared_ptr<Material> overrideMaterial);
   static const std::unordered_map<unsigned int, unsigned int> typeSizeLookUp;
   //size in bytes
   unsigned int vboDataSize;
   //length in strides
   unsigned int vboDataLength;
   unsigned int elementsLength;
   unsigned int vao;
   unsigned int vbo;
   unsigned int ebo; 
   std::shared_ptr<Material> mMaterial;
   std::function<void()> mUpdateLambda;
   bool mVisible;
   unsigned int CalculateVBOStride(const std::vector<VertexAttribute>& vertexAttributes);
   void Draw();
public:
   GLEntity(void* data, unsigned int _dataLength, std::vector<VertexAttribute>& vertexAttributes);
   GLEntity(void* data, unsigned int _dataLength, std::vector<VertexAttribute>& vertexAttributes, void* elements, unsigned int _elementsLength);
   ~GLEntity();
   int virtual Render(std::shared_ptr<Material> overrideMaterial = nullptr);
   void setVisible(const bool& visible);
   void setMaterial(std::shared_ptr<Material> material);
   void setUpdateLambda(std::function<void()> updateLambda);
};