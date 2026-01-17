#include "../headers/uniform.h"

Uniform::Uniform(Shader shaderProgram) {
    Uniform::shaderProgram = &shaderProgram;
}

void Uniform::SetToVec2(const char* uniform, const GLfloat* value) {
   GLuint uniformLoc = glGetUniformLocation(shaderProgram->ID, uniform);
   glUniform2fv(uniformLoc, 1, &value[0]);
}

void Uniform::SetToVec3(const char* uniform, const GLfloat* value) {
   GLuint uniformLoc = glGetUniformLocation(shaderProgram->ID, uniform);
   glUniform3fv(uniformLoc, 1, &value[0]);
}

void Uniform::SetToFloat(const char* uniform, GLfloat value) {
    GLuint uniformLoc = glGetUniformLocation(shaderProgram->ID, uniform);
    glUniform1f(uniformLoc, value);
}