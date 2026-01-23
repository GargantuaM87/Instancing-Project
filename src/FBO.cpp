#include "../headers/FBO.h"

FBO::FBO()
{
    glGenFramebuffers(1, &ID);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

void FBO::AttatchTexture(int width, int height)
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}   

void FBO::AttatchRenderBuffer(GLenum internalFormat, GLenum attatchmentType, int width, int height)
{
    glGenRenderbuffers(1, &renderbufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attatchmentType, GL_RENDERBUFFER, renderbufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void FBO::CheckStatus() 
{
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
}

void FBO::BindTexture()
{
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void FBO::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

void FBO::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::Delete()
{
    glDeleteFramebuffers(1, &ID);
    glDeleteTextures(1, &textureID);
    glDeleteRenderbuffers(1, &renderbufferID);
}