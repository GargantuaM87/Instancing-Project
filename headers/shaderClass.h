#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include "../src/glad.h"
#include "../include/glm/glm.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const char* filename);

class Shader {
    public:
        GLuint ID;
        Shader(const char* vertexFile, const char* fragmentFile);

        void LinkGeometry(const char* geometryFile);

        void SetToVec2(const char* uniform, const GLfloat* value);

        void SetToVec3(const char* uniform, const GLfloat* value);

        void SetToFloat(const char* uniform, GLfloat value);

        void SetToInt(const char* uniform, GLint value);

        void SetToMat4(const char* uniform, glm::mat4 &matrix);

        void Activate();
        void Delete();
};


#endif