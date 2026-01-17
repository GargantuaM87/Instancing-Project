#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include "../src/glad.h"
#include "../include/stb/stb_image.h"

#include "shaderClass.h"

class TextureUnit {
    public:
        GLuint ID;
        GLenum type;

        TextureUnit(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

        // Assigns a texture unit to a texture
        void texUnit(Shader& shader, const char* uniform, GLuint unit);
        //Binds a texture
        void Bind();
        // Unbinds a texture
        void Unbind();
        // Deletes a texture
        void Delete();

};

#endif