#pragma once
#include <memory>
#include "Textures/Texture2D.h"

class FrameBuffer
{
public:
   FrameBuffer(unsigned int width, unsigned int height, bool useDepthStencilTexture = false);
   ~FrameBuffer();

   void bindFrameBuffer();
   void unbindFrameBuffer();
   void bindRenderBuffer();
   void unbindRenderBuffer();
   void bind();
   void unbind();
   bool isComplete();
   bool hasDepthTexture();
   const Texture2D* getColourTexture();
   const Texture2D* getDepthTexture();
   void resize(int width, int height);

private: 
   unsigned int fbo;
   unsigned int rbo;
   Texture2D colourTexture;
   Texture2D depthTexture;
};

