#pragma once

#include "mesh.h"
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class ShaderProgram;
class Model
{
public:
    Model(const std::string path);
    void draw(ShaderProgram& shader);

private:
    void                 loadModel(const std::string& path);
    void                 processNode(aiNode* node, const aiScene* scene);
    Mesh                 processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType* type, std::string typeName);

private:
    std::vector<Mesh>    m_meshes;
    std::string          m_directory;
    std::vector<Texture> m_loadedTextures;
};