#pragma once
#include "glad/glad.h"
#include "../utils/glutils/GLUtils.h"

class Texture
{
private:
   GLuint textureId;
public:
   Texture(const char* fileName, GLPixelDataFormat format, const bool flip = false, const char* assetPath = "assets\\textures\\");
   ~Texture();
   void use(int textureUnitIndex = GL_TEXTURE0);
};

