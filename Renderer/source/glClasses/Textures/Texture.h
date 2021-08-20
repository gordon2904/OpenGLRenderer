#pragma once
#include <glad/glad.h>
#include "../../utils/glutils/GLUtils.h"

class Texture
{
protected:
   GLuint textureId;
   void setFlipVertical(bool value);
   unsigned char* loadImage(const char* filePath, int& height, int& width, int& channelCount);
   void freeImage(void* imageData);
public:
   Texture();
   virtual ~Texture();
   GLuint getTextureId();
   void use(int textureUnitIndex = GL_TEXTURE0) const;
   void virtual bind() const = 0;
   void generateTexture();
};

