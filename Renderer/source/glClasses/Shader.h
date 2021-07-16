#pragma once
#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
   unsigned int programId;

   Shader(const char* shaderName);
   ~Shader();

   void use() const;

   //uniform setters
   void setBool(const std::string &name, bool value) const;
   void setInt(const std::string& name, int value) const;
   void setFloat(const std::string& name, float value) const;
   void setVec2(const std::string& name, float values[2]) const;
   void setVec3(const std::string& name, float values[3]) const;
   void setVec4(const std::string& name, float values[4]) const;
};

