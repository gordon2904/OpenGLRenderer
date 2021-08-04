#pragma once
#include <memory>
#include "Textures/Texture.h"

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
   std::shared_ptr<Texture> getTexture();
   std::shared_ptr<Texture> getDepthTexture();
   void resize(int width, int height);

private: 
   unsigned int fbo;
   unsigned int rbo;
   std::shared_ptr<Texture> texture;
   std::shared_ptr<Texture> depthTexture;
};

