#pragma once
#include "glad/glad.h"
#include "../utils/glutils/GLUtils.h"

class Texture
{
private:
   GLuint textureId;
public:
   Texture(const char* fileName, const bool flip = false, GLPixelDataFormat format = GLPixelDataFormat::NONE, const char* assetPath = "assets\\textures\\");
   ~Texture();
   GLuint getTextureId();
   void use(int textureUnitIndex = GL_TEXTURE0);
};

