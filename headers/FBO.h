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
        void CheckStatus();
        void BindTexture();
        void Bind();
        void Unbind();
        void Delete();

};

#endif