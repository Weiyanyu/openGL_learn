#include "model.h"
#include "log.h"
#include "shader.h"
#include <algorithm>

Model::Model(const std::string path)
{
    loadModel(path);
}

void Model::draw(ShaderProgram& shader)
{
    for(size_t i = 0; i < m_meshes.size(); i++)
    {
        m_meshes[i].draw(shader);
    }
}

void Model::loadModel(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene*   scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        GL_LOG_E("Failed to load model: %s", importer.GetErrorString());
        return;
    }

    m_directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for(size_t i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }

    for(size_t i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;

    // vertex
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        // position
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        // normal
        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;

        // texcoords
        vertex.texCoords = glm::vec2(0.0);
        if(mesh->mTextureCoords[0])
        {
            vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
        }

        vertices.push_back(vertex);
    }

    // indices
    for(size_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(size_t j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // material
    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial*          material    = scene->mMaterials[mesh->mMaterialIndex];
        aiTextureType        type        = aiTextureType_DIFFUSE;
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, &type, "texture_diffuse");

        textures.insert(textures.end(), std::make_move_iterator(diffuseMaps.begin()), std::make_move_iterator(diffuseMaps.end()));
        type                              = aiTextureType_SPECULAR;
        std::vector<Texture> specularMaps = loadMaterialTextures(material, &type, "texture_specular");
        textures.insert(textures.end(), std::make_move_iterator(specularMaps.begin()), std::make_move_iterator(specularMaps.end()));
    }

    return Mesh(vertices, indices, textures);
}
std::vector<Texture> Model::loadMaterialTextures(aiMaterial* material, aiTextureType* type, std::string typeName)
{
    std::vector<Texture> textures;
    for(size_t i = 0; i < material->GetTextureCount(*type); i++)
    {
        aiString str;
        material->GetTexture(*type, i, &str);
        TextureType textureType;
        switch(*type)
        {
        case aiTextureType_DIFFUSE:
            textureType = TextureType::TEXTURE_DIFFUSE;
            break;
        case aiTextureType_SPECULAR:
            textureType = TextureType::TEXTURE_SPECULAR;
            break;
        default:
            GL_LOG_E("don't support texture %d yet", *type);
            std::abort();
        }

        std::string fullPath      = m_directory + "/" + std::string(str.C_Str());
        auto        loadedTexture = std::find_if(m_loadedTextures.begin(), m_loadedTextures.end(), [&fullPath](Texture& texture) { return texture.path() == fullPath; });
        if(loadedTexture == m_loadedTextures.end())
        {
            textures.emplace_back(fullPath, textureType, false);
            m_loadedTextures.push_back(textures.back());
        }
        else
        {
            textures.push_back(*loadedTexture);
        }
    }
    return textures;
}