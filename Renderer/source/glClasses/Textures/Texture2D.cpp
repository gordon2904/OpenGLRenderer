#include "Texture2D.h"
#include <glad/glad.h>
#include <string>
#include <sstream>
#include "../../utils/logger/Logger.h"

Texture2D::Texture2D(const char* fileName, const bool flip, GLPixelDataFormat format, const char* assetPath) : Texture()
{ 
   //create path string
   std::stringstream pathStringStream;
   pathStringStream << assetPath << fileName;
   const std::string pathString = pathStringStream.str();

   //generate GLuint texture id
   generateTexture();
   bind();

   // set wrapping and filtering options to our newly bound texture id

   // s, t, r meaning x, y, z coord on texture, this is set to repeat the texture across x and y
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   // apply linear texture filtering when the texture minifies or magnifies
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   int width, height, channelCount;
   setFlipVertical(flip);
   unsigned char* textureData = loadImage(pathString.c_str(), width, height, channelCount);
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
   freeImage(textureData);
}

Texture2D::Texture2D() : Texture() {}

Texture2D::~Texture2D()
{

}

void Texture2D::bind() const
{
   glBindTexture(GL_TEXTURE_2D, textureId);
}