#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "../src/glad.h"
#include <iostream>

class FBO
{
    public:
        GLuint ID;
        GLuint textureID;
        GLuint renderbufferID;

        FBO();
        void AttatchTexture(int width, int height);
        void AttatchRenderBuffer(GLenum internalFormat, GLenum attatchmentType, int width, int height);
        void AttatchTextureMSAA(int samples, int width, int height);
        void AttatchRenderBufferMSAA(int samples, GLenum internalFormat, GLenum attatchmentType, int width, int height);
        void Blit(int width, int height);
        void CheckStatus();
        void BindTexture();
        void BindTextureMSAA();
        void Bind();
        void Unbind();
        void Delete();

};

#endif