#pragma once
#include "Texture.h"

class Texture2D : public Texture
{
public:
   Texture2D(const char* fileName, const bool flip = false, GLPixelDataFormat format = GLPixelDataFormat::NONE, const char* assetPath = "assets\\textures\\");
   Texture2D();
   virtual ~Texture2D();
   virtual void bind();
};

