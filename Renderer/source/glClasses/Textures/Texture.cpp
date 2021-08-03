#include "Texture.h"
#include <glad/glad.h>
#include "../../utils/logger/Logger.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture()
{
   //generate GLuint texture id
   glGenTextures(1, &textureId);
}


GLuint Texture::getTextureId()
{
   return textureId;
}

Texture::~Texture()
{

}

void Texture::use(int textureUnitIndex)
{
   if(textureUnitIndex >= GL_TEXTURE0 && textureUnitIndex < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS)
   {
      glActiveTexture(textureUnitIndex);
      bind();
   }
   else
   {
      LOG_ERROR("Texture attempting to use invalid texutre Unit index: {0}", textureUnitIndex);
   }
}

void Texture::setFlipVertical(bool value)
{
   stbi_set_flip_vertically_on_load(value);
}


unsigned char* Texture::loadImage(const char* filePath, int& height, int& width, int& channelCount)
{
   return stbi_load(filePath, &width, &height, &channelCount, 0);
}

void Texture::freeImage(void* imageData)
{
   stbi_image_free(imageData);
}