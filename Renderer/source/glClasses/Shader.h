#pragma once
#include <string>

class Shader
{
private:
   unsigned int programId;
public:
   const unsigned int getProgramId();
   Shader(const char* shaderName);
   ~Shader();

   void use() const;
};

