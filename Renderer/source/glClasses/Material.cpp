#include "Material.h"
#include "../utils/logger/Logger.h"
#include <glm/gtc/type_ptr.hpp>

const int minTextureUnit = 0;
const int maxTextureUnit = GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS - GL_TEXTURE0;

Material::Material(std::shared_ptr<Shader> shader) : mShader(shader)
{

}

void Material::setTexture(const char* uniformName, const Texture* texture, unsigned int textureUnit)
{
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

std::unordered_map<unsigned int, const Texture*>* Material::getTextures()
{
   return &texturesMap;
}

void Material::use() const
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