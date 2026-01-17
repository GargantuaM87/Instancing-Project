#include "../headers/texture.h"

TextureUnit::TextureUnit(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType) {
    type = texType;

    // Stores the width, height and number of color channels of the image
    int widthImg, heightImg, numColCh;
    // Flips the image so it appears right side up
    stbi_set_flip_vertically_on_load(true);
    // Reads the image from a file and stores it in bytes
    unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, STBI_rgb_alpha);
    
    // Generates an OpenGL texture object
    glGenTextures(1, &ID);
    // Assigns texture to texture unit
    glActiveTexture(slot);
    glBindTexture(texType, ID);

    // Configure the type of alg// Extra lines in case you choose to use GL_CLAMP_TO_BORDER
	// float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);orithm that is used to determine how the image gets smaller/bigger
    glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Configure the way the texture repeats, it it does at all
    glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Extra lines in case you choose to use GL_CLAMP_TO_BORDER
	// float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

    // Assigns the image to the OpenGL object
    glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
    // Generate Mipmaps
    glGenerateMipmap(texType);

    // Deletes the image data as it is already used in the OpenGL texture object
    stbi_image_free(bytes);

    // Unbind the texture object so nothing else can happen to it
    glBindTexture(texType, 0);
}

void TextureUnit::texUnit(Shader& shader, const char* uniform, GLuint unit) {
    // Gets the location of the uniform
    GLuint texUni = glGetUniformLocation(shader.ID, uniform);
    // Shader needs to be activated before changing the value of a uniform
    shader.Activate();
    // Set value of the uniform
    glUniform1i(texUni, unit);
}

void TextureUnit::Bind() {
    glBindTexture(type, ID);
}

void TextureUnit::Unbind() {
    glBindTexture(type, 0);
}

void TextureUnit::Delete() {
    glDeleteTextures(1, &ID);
}