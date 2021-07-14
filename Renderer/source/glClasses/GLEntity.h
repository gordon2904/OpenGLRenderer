#ifndef GLENTITY_H
#define GLENTITY_H
#include<functional>
class GLEntity
{
protected:
   unsigned int vao;
   unsigned int vbo;
   unsigned int mShaderProgram;
   std::function<void()> mUpdateLambda;
   bool mVisible;
   GLEntity();
public:
   ~GLEntity();
   int virtual Render(const unsigned int shaderProgram = 0);
   void setVisible(const bool& visible);
   void setShaderProgram(unsigned int shaderProgram);
   unsigned int getShaderProgram();
   void setUpdateLambda(std::function<void()> updateLambda);
};
#endif