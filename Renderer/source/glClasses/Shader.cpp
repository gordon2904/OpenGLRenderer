#include "Shader.h"
#include <glad/glad.h>
#include <sstream>
#include <fstream>
#include "../utils/logger/Logger.h"
#include <string>

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