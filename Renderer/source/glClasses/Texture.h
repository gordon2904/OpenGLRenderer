#pragma once
#include "glad/glad.h"

class Texture
{
private:
   GLuint textureId;
public:
   Texture(const char* fileName, const char* assetPath = "assets\\textures\\");
   ~Texture();
   void use(int textureUnitIndex = GL_TEXTURE0);
};

