#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "../include/stb/stb_image.h"
#include "shaderClass.h"
#include "mesh.h"

class Model
{
    public:
        std::vector<Mesh> meshes;
        std::vector<Texture> textures_loaded;
        Model(char *path) 
        {
            LoadModel(path);
        }
        void Draw(Shader &shader);
    private:
        // model data
        
        std::string directory;

        void LoadModel(std::string path);
        void ProcessNode(aiNode *node, const aiScene *scene);
        Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
        unsigned int TextureFromFile(const char *path, const std::string &directory);
        
};

#endif