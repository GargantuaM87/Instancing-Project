#include "../headers/shaderClass.h"

// char* filename represents a character array
// In C, there is no built-in string class, so this is used to represent strings as well
std::string get_file_contents(const char* filename) {
    std::ifstream in(filename, std::ios::binary); // Creates an input file stream that reads from the file in binary, so automatic conversions don't happen
    if (in) {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return(contents);
    }
    throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile) 
{
    std::string vertexCode = get_file_contents(vertexFile);
    std::string fragmentCode = get_file_contents(fragmentFile);

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    // This is an unsigned-int 
   // glShaderSource replaces the source code of vertexShader with the source code of vertexShaderSource (its memory address)
   // GPU can't understand source code, so we compile the shader as well
   GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertexShader, 1, &vertexSource, NULL);
   glCompileShader(vertexShader);

   GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
   glCompileShader(fragmentShader);

   // Wrap the fragment and vertex shaders into a shader program
   ID = glCreateProgram();
   glAttachShader(ID, vertexShader);
   glAttachShader(ID, fragmentShader);
   glLinkProgram(ID);

   // Delete the shaders since we now have them in our shader program (memory management!)
   glDeleteShader(vertexShader);
   glDeleteShader(fragmentShader);
}

void Shader::LinkGeometry(const char* geometryFile) {
    std::string geoCode = get_file_contents(geometryFile);
    const char* geoSource = geoCode.c_str();

    GLuint geoShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geoShader, 1, &geoSource, NULL);
    glCompileShader(geoShader);

    glAttachShader(ID, geoShader);
    glLinkProgram(ID);

    glDeleteShader(geoShader);
}

void Shader::SetToVec2(const char* uniform, const GLfloat* value) {
   GLuint uniformLoc = glGetUniformLocation(ID, uniform);
   glUniform2fv(uniformLoc, 1, &value[0]);
}

void Shader::SetToVec3(const char* uniform, const GLfloat* value) {
   GLuint uniformLoc = glGetUniformLocation(ID, uniform);
   glUniform3fv(uniformLoc, 1, &value[0]);
}

void Shader::SetToFloat(const char* uniform, GLfloat value) {
    GLuint uniformLoc = glGetUniformLocation(ID, uniform);
    glUniform1f(uniformLoc, value);
}

void Shader::SetToInt(const char* uniform, GLint value) {
    GLuint uniformLoc = glGetUniformLocation(ID, uniform);
    glUniform1i(uniformLoc, value);
}

void Shader::SetToMat4(const char* uniform, glm::mat4 &matrix) {
    glUniformMatrix4fv(glGetUniformLocation(ID, uniform), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::Activate() 
{
    glUseProgram(ID);
}

void Shader::Delete() 
{
    glDeleteProgram(ID);
}
