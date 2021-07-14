#include "GLEntity.h"
#include <glad/glad.h>
#include "../utils/logger/Logger.h"

GLEntity::GLEntity() : mVisible(true)
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

int GLEntity::Render(const unsigned int &shaderProgram)
{
   glUseProgram(shaderProgram);
   glBindVertexArray(vao);
   return 1;
}

void GLEntity::setVisible(const bool& visible)
{
   mVisible = visible;
}