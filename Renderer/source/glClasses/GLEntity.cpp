#include "GLEntity.h"
#include <glad/glad.h>
#include "../utils/logger/Logger.h"

GLEntity::GLEntity() : mVisible(true), mShaderProgram(0), mUpdateLambda([]() {})
{
   glGenVertexArrays(1, &vao);
   glGenBuffers(1, &vbo);
}

GLEntity::~GLEntity()
{
   LOG_INFO("deleting\nvao: {0}\nvbo: {1}", vao, vbo);
   glDeleteVertexArrays(1, &vao);
   glDeleteBuffers(1, &vbo);
}

int GLEntity::Render(const unsigned int shaderProgram)
{
   if(!mVisible)
   {
      return 0;
   }
   mUpdateLambda();
   if(shaderProgram > 0)
   {
      glUseProgram(shaderProgram);
   }
   else if(mShaderProgram > 0)
   {
      glUseProgram(mShaderProgram);
   }
   else
   {
      return 0;
   }
   glBindVertexArray(vao);
   return 1;
}

void GLEntity::setVisible(const bool& visible)
{
   mVisible = visible;
}


void GLEntity::setShaderProgram(unsigned int shaderProgram)
{
   mShaderProgram = shaderProgram;
}

unsigned int GLEntity::getShaderProgram()
{
   return mShaderProgram;
}


void GLEntity::setUpdateLambda(std::function<void()> updateLambda)
{
   mUpdateLambda = updateLambda;
}