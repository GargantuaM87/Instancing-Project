#ifndef UNIFORM_CLASS_H
#define UNIFORM_CLASS_H

#include "shaderClass.h"

class Uniform {
    public:
        Shader* shaderProgram;

        Uniform(Shader shaderProgram);

        void SetToVec2(const char* uniform, const GLfloat* value);

        void SetToVec3(const char* uniform, const GLfloat* value);

        void SetToFloat(const char* uniform, GLfloat value);
};


#endif