#include "Texture.h"
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <string>
#include <sstream>
#include "../utils/logger/Logger.h"

Texture::Texture(const char* fileName, const bool flip, GLPixelDataFormat format, const char* assetPath)
{ 
   //create path string
   std::stringstream pathStringStream;
   pathStringStream << assetPath << fileName;
   const std::string pathString = pathStringStream.str();

   //generate GLuint texture id
   glGenTextures(1, &textureId);
   glBindTexture(GL_TEXTURE_2D, textureId);

   // set wrapping and filtering options to our newly bound texture id

   // s, t, r meaning x, y, z coord on texture, this is set to repeat the texture across x and y
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   // apply linear texture filtering when the texture minifies or magnifies
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   int width, height, channelCount;
   stbi_set_flip_vertically_on_load(flip);
   unsigned char* textureData = stbi_load(pathString.c_str(), &width, &height, &channelCount, 0);
   if(textureData)
   {
      if(format == GLPixelDataFormat::NONE)
      {
         if(channelCount == 1)
            format = GLPixelDataFormat::RED;
         else if(channelCount == 3)
            format = GLPixelDataFormat::RGB;
         else if(channelCount == 4)
            format = GLPixelDataFormat::RGBA;
      }
      glTexImage2D(GL_TEXTURE_2D, 0, channelCount == 4 ? GL_RGBA : GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
      glGenerateMipmap(GL_TEXTURE_2D);
   }
   else
   {
      LOG_ERROR("Failed to load texture at path: {0}", pathString);
   }
   stbi_image_free(textureData);
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
      glBindTexture(GL_TEXTURE_2D, textureId);
   }
   else
   {
      LOG_ERROR("Texture attempting to use invalid texutre Unit index: {0}", textureUnitIndex);
   }
}