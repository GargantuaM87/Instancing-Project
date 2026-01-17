#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include "../include/glm/glm.hpp"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include <string>
#include <vector>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};
struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
    public:
         unsigned int VAO, VBO, EBO;
        // Mesh Data
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        void Draw(Shader &shader);

    private:
        // Render Data

        void SetUpMesh();

};

#endif
