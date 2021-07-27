#pragma once
#include <string>
#include <glm/glm.hpp>

class Shader
{
private:
   unsigned int programId;
public:
   const unsigned int getProgramId();
   Shader(const char* shaderName);
   ~Shader();

   void use() const;
   void setBool(const std::string& name, bool value) const;
   void setInt(const std::string& name, int value) const;
   void setFloat(const std::string& name, float value) const;
   void setVec2(const std::string& name, glm::vec2 values) const;
   void setVec2(const std::string& name, float values[2]) const;
   void setVec2(const std::string& name, float x, float y) const;
   void setVec3(const std::string& name, glm::vec3 values) const;
   void setVec3(const std::string& name, float values[3]) const;
   void setVec3(const std::string& name, float x, float y, float z) const;
   void setVec4(const std::string& name, float values[4]) const;
   void setVec4(const std::string& name, glm::vec4 values) const;
   void setVec4(const std::string& name, float x, float y, float z, float w) const;
   void setMat4(const std::string& name, const glm::mat4& transform) const;
   void setMat3(const std::string& name, const glm::mat3& transform) const;
};

