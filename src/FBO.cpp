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

void FBO::AttatchTextureMSAA(int samples, int width, int height)
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureID);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width, height, GL_TRUE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureID, 0);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}

void FBO::AttatchRenderBufferMSAA(int samples, GLenum internalFormat, GLenum attatchmentType, int width, int height)
{
    glGenRenderbuffers(1, &renderbufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbufferID);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalFormat, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attatchmentType, GL_RENDERBUFFER, renderbufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void FBO::Blit(int width, int height)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, ID);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
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

void FBO::BindTextureMSAA()
{
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureID);
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