#pragma once
#include "glad/glad.h"
#include <memory>
#include <unordered_map>
#include "Shader.h"
#include "Texture.h"



class Material
{
private:
   std::shared_ptr<Shader> mShader;
   std::unordered_map<unsigned int, std::shared_ptr<Texture>> texturesMap;
public:
   void setTexture(const char* uniformName, std::shared_ptr<Texture> texture, unsigned int textureUnit = GL_TEXTURE0);
   std::unordered_map<unsigned int, std::shared_ptr<Texture>>* getTextures();
   const std::shared_ptr<Shader> getShader();
   void use();
   Material(std::shared_ptr<Shader> shader);


   //uniform setters
   void setBool(const std::string& name, bool value) const;
   void setInt(const std::string& name, int value) const;
   void setFloat(const std::string& name, float value) const;
   void setVec2(const std::string& name, float values[2]) const;
   void setVec3(const std::string& name, float values[3]) const;
   void setVec4(const std::string& name, float values[4]) const;
};

