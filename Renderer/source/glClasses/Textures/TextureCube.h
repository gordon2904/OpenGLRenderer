#pragma once
#include "Texture.h"

class TextureCube : public Texture
{
public:
   TextureCube(std::string fileNames[6], const bool flip = false, GLPixelDataFormat format = GLPixelDataFormat::NONE, const char* assetPath = "assets\\cubemaps\\");
   TextureCube();
   virtual ~TextureCube();
   virtual void bind() const;
};


