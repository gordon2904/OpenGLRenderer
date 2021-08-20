#include "FrameBuffer.h"
#include "glad/glad.h"
#include "../utils/logger/Logger.h"
#include "Textures/Texture2D.h"

FrameBuffer::FrameBuffer(unsigned int width, unsigned int height, bool useDepthStencilTexture) : fbo(0), rbo(0)
{
   glGenFramebuffers(1, &fbo);
   bindFrameBuffer();

   colourTexture.generateTexture();
   colourTexture.bind();

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colourTexture.getTextureId(), 0);

   if(useDepthStencilTexture)
   {
      depthTexture.generateTexture();
      depthTexture.bind();

      glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthTexture.getTextureId(), 0);
   }
   else
   {
      glGenRenderbuffers(1, &rbo);
      bindRenderBuffer();
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
   }
   if(!isComplete())
   {
      LOG_ERROR("FrameBuffer: Framebuffer is not complete!");
   }
   unbind();
}

void FrameBuffer::resize(int width, int height)
{
   colourTexture.bind();
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
   if(hasDepthTexture())
   {
      depthTexture.bind();
      glTexImage2D(
         GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0,
         GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr
      );
   }
   else
   {
      bindRenderBuffer();
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
   }
}

FrameBuffer::~FrameBuffer()
{
   glDeleteFramebuffers(1, &fbo);
}

void FrameBuffer::bind()
{
   bindFrameBuffer();
   bindRenderBuffer();
}

void FrameBuffer::unbind()
{
   unbindFrameBuffer();
   unbindRenderBuffer();
}

void FrameBuffer::bindFrameBuffer()
{
   glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void FrameBuffer::unbindFrameBuffer()
{
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bindRenderBuffer()
{
   if(!hasDepthTexture())
   {
      glBindRenderbuffer(GL_RENDERBUFFER, rbo);
   }
}

void FrameBuffer::unbindRenderBuffer()
{
   glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

bool FrameBuffer::isComplete()
{
   return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

bool FrameBuffer::hasDepthTexture()
{
   return depthTexture.getTextureId() > 0;
}

const Texture2D* FrameBuffer::getColourTexture()
{
   return &colourTexture;
}

const Texture2D* FrameBuffer::getDepthTexture()
{
   return &depthTexture;
}