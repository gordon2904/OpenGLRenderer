#pragma once
#include<functional>
#include <unordered_map>
#include<array>
#include "../utils/glutils/GLUtils.h"
#include "Shader.h"
class GLEntity
{
protected:
   static const std::unordered_map<unsigned int, unsigned int> typeSizeLookUp;
   unsigned int vboDataLength;
   unsigned int elementsLength;
   unsigned int vao;
   unsigned int vbo;
   unsigned int ebo; 
   std::shared_ptr<Shader> shader;
   std::function<void()> mUpdateLambda;
   bool mVisible;
   unsigned int CalculateVBOStride(const std::vector<VertexAttribute>& vertexAttributes);
   void Draw();
public:
   GLEntity(void* data, unsigned int _dataLength, std::vector<VertexAttribute>& vertexAttributes);
   GLEntity(void* data, unsigned int _dataLength, std::vector<VertexAttribute>& vertexAttributes, void* elements, unsigned int _elementsLength);
   ~GLEntity();
   int virtual Render(std::shared_ptr<Shader> overrideShader = nullptr);
   void setVisible(const bool& visible);
   void setShaderProgram(std::shared_ptr<Shader> shader);
   std::shared_ptr<Shader> getShaderProgram();
   void setUpdateLambda(std::function<void()> updateLambda);
};