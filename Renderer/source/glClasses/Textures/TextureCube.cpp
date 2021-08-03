#include "TextureCube.h"
#include <glad/glad.h>
#include <string>
#include <sstream>
#include "../../utils/logger/Logger.h"

const unsigned int textureOrder[] = {
   GL_TEXTURE_CUBE_MAP_POSITIVE_X,
   GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
   GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
   GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
   GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
   GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
};

TextureCube::TextureCube(std::string fileNames[6], const bool flip, GLPixelDataFormat format, const char* assetPath)
{
   //generate GLuint texture id
   glGenTextures(1, &textureId);
   bind();

   int width, height, channelCount; 
   unsigned char* textureData;
   setFlipVertical(flip);
   for(unsigned int i = 0; i < 6; ++i)
   {
      const std::string pathString = assetPath + fileNames[i];
      textureData = loadImage(pathString.c_str(), width, height, channelCount);
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
         glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, channelCount == 4 ? GL_RGBA : GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
      }
      else
      {
         LOG_ERROR("Failed to load texture at path: {0}", pathString);
      }
      freeImage(textureData);
   }

   // set wrapping and filtering options to our newly bound texture id
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

TextureCube::TextureCube() : Texture() {}

TextureCube::~TextureCube()
{

}

void TextureCube::bind()
{
   glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
}