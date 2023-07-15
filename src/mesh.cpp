#include "mesh.h"
#include "log.h"
#include "shader.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures)
    : m_vertices(vertices)
    , m_indices(indices)
    , m_texture(textures)
{
    m_refCnt = new unsigned(1);
    setupMesh();
}

Mesh::Mesh(const Mesh& other)
{
    *this = other;
}
Mesh& Mesh::operator=(const Mesh& other)
{
    if(this != &other)
    {
        m_vertices = other.m_vertices;
        m_indices  = other.m_indices;
        m_texture  = other.m_texture;
        m_VAO      = other.m_VAO;
        m_VBO      = other.m_VBO;
        m_EBO      = other.m_EBO;
        (*other.m_refCnt)++;
        m_refCnt = other.m_refCnt;
        (*m_refCnt)++;
    }
    return *this;
}
Mesh::Mesh(Mesh&& other)
{
    *this = std::move(other);
}
Mesh& Mesh::operator=(Mesh&& other)
{
    if(this != &other)
    {
        m_vertices = std::move(other.m_vertices);
        m_indices  = std::move(other.m_indices);
        m_texture  = std::move(other.m_texture);
        m_VAO      = other.m_VAO;
        m_VBO      = other.m_VBO;
        m_EBO      = other.m_EBO;
        m_refCnt   = other.m_refCnt;

        other.m_VAO    = 0;
        other.m_EBO    = 0;
        other.m_VBO    = 0;
        other.m_refCnt = nullptr;
    }
    return *this;
}

void Mesh::draw(ShaderProgram& shader)
{
    unsigned int diffuseNr  = 0;
    unsigned int specularNr = 0;
    shader.use();
    for(size_t i = 0; i < m_texture.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number      = "";
        auto        textureType = m_texture[i].type();
        switch(textureType)
        {
        case TextureType::TEXTURE_DIFFUSE:
            diffuseNr++;
            break;
        case TextureType::TEXTURE_SPECULAR:
            specularNr++;
            break;
        default:
            GL_LOG_E("don't support texture type %d yet", textureType);
            std::abort();
            break;
        }
        std::string textureName = "material" + std::to_string(diffuseNr) + "." + Texture::translateTextureTypeName(textureType);
        shader.setInt(textureName, i);
        glBindTexture(GL_TEXTURE_2D, m_texture[i].id());
    }
    // draw mesh
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(m_indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

Mesh::~Mesh()
{
    if(m_refCnt)
    {
        (*m_refCnt)--;
        if(*m_refCnt == 0)
        {
            GL_LOG_D("release vao %d vbo %d ebo %d", m_VAO, m_VBO, m_EBO);
            glDeleteBuffers(1, &m_VBO);
            glDeleteBuffers(1, &m_EBO);
            glDeleteVertexArrays(1, &m_VAO);
        }
    }
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}