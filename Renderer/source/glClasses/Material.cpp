#include "Material.h"
#include "../utils/logger/Logger.h"
#include <glm/gtc/type_ptr.hpp>

const int minTextureUnit = 0;
const int maxTextureUnit = GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS - GL_TEXTURE0;

Material::Material(std::shared_ptr<Shader> shader) : mShader(shader)
{

}

void Material::setTexture(const char* uniformName, std::shared_ptr<Texture> texture, unsigned int textureUnit)
{
   LOG_INFO("Min: {0}, Max: {1}.\nTexture Unit: {2}", GL_TEXTURE0, GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, textureUnit);
   if(textureUnit >= GL_TEXTURE0 && textureUnit < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS)
   {
      texturesMap[textureUnit] = texture;
      mShader->use();
      LOG_INFO("Setting texture unit: {0}", textureUnit - GL_TEXTURE0);
      setInt(uniformName, textureUnit - GL_TEXTURE0);
   }
   else
   {
      LOG_ERROR("Failed to set texture with invalid texture unit index: {0}", textureUnit);
   }
}

std::unordered_map<unsigned int, std::shared_ptr<Texture>>* Material::getTextures()
{
   return &texturesMap;
}

void Material::use()
{
   mShader->use();
   if(texturesMap.size() > 0)
   {
      for(auto& texture : texturesMap)
      {
         texture.second->use(texture.first);
      }
   }
}

const std::shared_ptr<Shader> Material::getShader()
{
   return mShader;
}

void Material::setBool(const std::string& name, bool value) const
{
   glUniform1i(glGetUniformLocation(mShader->getProgramId(), name.c_str()), (int)value);
}

void Material::setInt(const std::string& name, int value) const
{
   glUniform1i(glGetUniformLocation(mShader->getProgramId(), name.c_str()), value);
}

void Material::setFloat(const std::string& name, float value) const
{
   glUniform1f(glGetUniformLocation(mShader->getProgramId(), name.c_str()), value);
}

void Material::setVec2(const std::string& name, float values[2]) const
{
   setVec2(name, values[0], values[1]);
}
void Material::setVec2(const std::string& name, glm::vec2 values) const
{
   setVec2(name, values[0], values[1]);
}
void Material::setVec2(const std::string& name, float x, float y) const
{
   glUniform2f(glGetUniformLocation(mShader->getProgramId(), name.c_str()), x, y);
}

void Material::setVec3(const std::string& name, float values[3]) const
{
   setVec3(name, values[0], values[1], values[2]);
}
void Material::setVec3(const std::string& name, glm::vec3 values) const
{
   setVec3(name, values[0], values[1], values[2]);
}
void Material::setVec3(const std::string& name, float x, float y, float z) const
{
   glUniform3f(glGetUniformLocation(mShader->getProgramId(), name.c_str()), x, y, z);
}

void Material::setVec4(const std::string& name, float values[4]) const
{
   setVec4(name, values[0], values[1], values[2], values[3]);
}
void Material::setVec4(const std::string& name, glm::vec4 values) const
{
   setVec4(name, values[0], values[1], values[2], values[3]);
}
void Material::setVec4(const std::string& name, float x, float y, float z, float w) const
{
   glUniform4f(glGetUniformLocation(mShader->getProgramId(), name.c_str()), x, y, z, w);
}

void Material::setMat4(const std::string& name, const glm::mat4& transform) const
{
   glUniformMatrix4fv(glGetUniformLocation(mShader->getProgramId(), name.c_str()), 1, GL_FALSE, glm::value_ptr(transform));
}
void Material::setMat3(const std::string& name, const glm::mat3& transform) const
{
   glUniformMatrix3fv(glGetUniformLocation(mShader->getProgramId(), name.c_str()), 1, GL_FALSE, glm::value_ptr(transform));
}