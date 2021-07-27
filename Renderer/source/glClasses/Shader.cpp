#include "Shader.h"
#include <glad/glad.h>
#include <sstream>
#include <fstream>
#include "../utils/logger/Logger.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


int createShader(GLuint& shaderId, GLuint shaderType, const char* shaderCode, char logInfo[512], int &success)
{
   shaderId = glCreateShader(shaderType);
   glShaderSource(shaderId, 1, &shaderCode, nullptr);
   glCompileShader(shaderId);
   glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
   if(!success)
   {
      glGetShaderInfoLog(shaderId, 512, NULL, logInfo);
      LOG_ERROR("Shader Compilation failed");
      return 0;
   };
   return 1;
}

Shader::Shader(const char* shaderName): programId(0)
{
   std::string vertexCode, fragmentCode;
   std::ifstream vertShaderFileStream, fragShaderFileStream;

   vertShaderFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
   fragShaderFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

   std::stringstream vertShaderPath;
   std::stringstream fragShaderPath;

   vertShaderPath << "assets\\shaders\\" << shaderName << "\\vert.glsl";
   fragShaderPath << "assets\\shaders\\" << shaderName << "\\frag.glsl";

   std::string vertexPath = vertShaderPath.str();
   std::string fragmentPath = fragShaderPath.str();

   try
   {
      //open files
      vertShaderFileStream.open(vertexPath);
      fragShaderFileStream.open(fragmentPath);
      std::stringstream vertShaderStringStream, fragShaderStringStream;

      //read file buffers into string streams
      vertShaderStringStream << vertShaderFileStream.rdbuf();
      fragShaderStringStream << fragShaderFileStream.rdbuf();

      //close files
      vertShaderFileStream.close();
      fragShaderFileStream.close();

      vertexCode = vertShaderStringStream.str();
      fragmentCode = fragShaderStringStream.str();
   }
   catch(std::ifstream::failure exception)
   {
      LOG_ERROR("Shader: {0}", exception.what());
   }

   const char* vShaderCode = vertexCode.c_str();
   const char* fShaderCode = fragmentCode.c_str();

   int success;
   char infoLog[512];
   GLuint vertexShader, fragmentShader;
   if(!createShader(vertexShader, GL_VERTEX_SHADER, vShaderCode, infoLog, success))
   {
      LOG_ERROR("Failed vertex shader compilation: {0}\n{1}", shaderName, infoLog);
      return;
   }
   if(!createShader(fragmentShader, GL_FRAGMENT_SHADER, fShaderCode, infoLog, success))
   {
      glDeleteShader(vertexShader);
      LOG_ERROR("Failed fragment shader compilation: {0}\n{1}", shaderName, infoLog);
      return;
   }

   programId = glCreateProgram();
   glAttachShader(programId, vertexShader);
   glAttachShader(programId, fragmentShader);
   glLinkProgram(programId);

   glGetProgramiv(programId, GL_LINK_STATUS, &success);
   if(!success)
   {
      glGetProgramInfoLog(programId, 512, nullptr, infoLog);
      LOG_ERROR("Failed linking shader: {0}\n{1}", shaderName, infoLog);
   }
   glDeleteShader(vertexShader);
   glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
   LOG_INFO("deleting Shader\nprogramId:{0}", programId);
   glDeleteProgram(programId);
}

void Shader::use() const
{
   glUseProgram(programId);
}


const unsigned int Shader::getProgramId()
{
   return programId;
}

void Shader::setBool(const std::string& name, bool value) const
{
   glUniform1i(glGetUniformLocation(programId, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
   glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
   glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, float values[2]) const
{
   setVec2(name, values[0], values[1]);
}
void Shader::setVec2(const std::string& name, glm::vec2 values) const
{
   setVec2(name, values[0], values[1]);
}
void Shader::setVec2(const std::string& name, float x, float y) const
{
   glUniform2f(glGetUniformLocation(programId, name.c_str()), x, y);
}

void Shader::setVec3(const std::string& name, float values[3]) const
{
   setVec3(name, values[0], values[1], values[2]);
}
void Shader::setVec3(const std::string& name, glm::vec3 values) const
{
   setVec3(name, values[0], values[1], values[2]);
}
void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
   glUniform3f(glGetUniformLocation(programId, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string& name, float values[4]) const
{
   setVec4(name, values[0], values[1], values[2], values[3]);
}
void Shader::setVec4(const std::string& name, glm::vec4 values) const
{
   setVec4(name, values[0], values[1], values[2], values[3]);
}
void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
   glUniform4f(glGetUniformLocation(programId, name.c_str()), x, y, z, w);
}

void Shader::setMat4(const std::string& name, const glm::mat4& transform) const
{
   glUniformMatrix4fv(glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, glm::value_ptr(transform));
}
void Shader::setMat3(const std::string& name, const glm::mat3& transform) const
{
   glUniformMatrix3fv(glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, glm::value_ptr(transform));
}