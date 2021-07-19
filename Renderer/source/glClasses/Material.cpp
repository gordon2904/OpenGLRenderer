#include "Material.h"
#include "../utils/logger/Logger.h"

const int minTextureUnit = 0;
const int maxTextureUnit = GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS - GL_TEXTURE0;

Material::Material(std::shared_ptr<Shader> shader) : mShader(shader)
{

}

void Material::setTexture(const char* uniformName, std::shared_ptr<Texture> texture, unsigned int textureUnit)
{
   if(textureUnit >= GL_TEXTURE0 && textureUnit < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS)
   {
      texturesMap[textureUnit] = texture;
      use();
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
   glUniform2f(glGetUniformLocation(mShader->getProgramId(), name.c_str()), values[0], values[1]);
}
void Material::setVec3(const std::string& name, float values[3]) const
{
   glUniform3f(glGetUniformLocation(mShader->getProgramId(), name.c_str()), values[0], values[1], values[2]);
}
void Material::setVec4(const std::string& name, float values[4]) const
{
   glUniform4f(glGetUniformLocation(mShader->getProgramId(), name.c_str()), values[0], values[1], values[2], values[3]);
}