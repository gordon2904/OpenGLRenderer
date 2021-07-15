#pragma once
#include<functional>
#include <unordered_map>
#include<array>
#include "../utils/glutils/GLUtils.h"
class GLEntity
{
protected:
   static const std::unordered_map<unsigned int, unsigned int> typeSizeLookUp;
   unsigned int vboDataLength;
   unsigned int elementsLength;
   unsigned int vao;
   unsigned int vbo;
   unsigned int ebo;
   unsigned int mShaderProgram;
   std::function<void()> mUpdateLambda;
   bool mVisible;
   unsigned int CalculateVBOStride(const std::vector<VertexAttribute>& vertexAttributes);
   void Draw();
public:
   GLEntity(void* data, unsigned int _dataLength, std::vector<VertexAttribute>& vertexAttributes);
   GLEntity(void* data, unsigned int _dataLength, std::vector<VertexAttribute>& vertexAttributes, void* elements, unsigned int _elementsLength);
   ~GLEntity();
   int virtual Render(const unsigned int shaderProgram = 0);
   void setVisible(const bool& visible);
   void setShaderProgram(unsigned int shaderProgram);
   unsigned int getShaderProgram();
   void setUpdateLambda(std::function<void()> updateLambda);
};