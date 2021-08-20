#pragma once
#include "glad/glad.h"
#include <memory>
#include <unordered_map>
#include "Shader.h"
#include "Textures/Texture.h"
#include <glm/glm.hpp>



class Material
{
private:
   std::shared_ptr<Shader> mShader;
   std::unordered_map<unsigned int, const Texture*> texturesMap;
public:
   void setTexture(const char* uniformName, const Texture* texture, unsigned int textureUnit = GL_TEXTURE0);
   std::unordered_map<unsigned int, const Texture*>* getTextures();
   const std::shared_ptr<Shader> getShader();
   void use() const;
   Material(std::shared_ptr<Shader> shader);


   //uniform setters
   inline void setBool(const std::string& name, bool value) const { mShader->setBool(name, value); }
   inline void setInt(const std::string& name, int value) const { mShader->setInt(name, value); }
   inline void setFloat(const std::string& name, float value) const { mShader->setFloat(name, value); }
   inline void setVec2(const std::string& name, glm::vec2 values) const { mShader->setVec2(name, values); }
   inline void setVec2(const std::string& name, float values[2]) const { mShader->setVec2(name, values); }
   inline void setVec2(const std::string& name, float x, float y) const { mShader->setVec2(name, x, y); }
   inline void setVec3(const std::string& name, glm::vec3 values) const { mShader->setVec3(name, values); }
   inline void setVec3(const std::string& name, float values[3]) const { mShader->setVec3(name, values); }
   inline void setVec3(const std::string& name, float x, float y, float z) const { mShader->setVec3(name, x, y, z); }
   inline void setVec4(const std::string& name, float values[4]) const { mShader->setVec4(name, values); }
   inline void setVec4(const std::string& name, glm::vec4 values) const { mShader->setVec4(name, values); }
   inline void setVec4(const std::string& name, float x, float y, float z, float w) const { mShader->setVec4(name, x, y ,z, w); }
   inline void setMat4(const std::string& name, const glm::mat4 &transform) const { mShader->setMat4(name, transform); }
   inline void setMat3(const std::string& name, const glm::mat3& transform) const { mShader->setMat3(name, transform); }
};

