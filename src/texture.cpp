#include "texture.h"
#include "log.h"
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format off

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::string Texture::translateTextureTypeName(TextureType textureType)
{
    std::string textureTypename = "";
    switch (textureType)
    {
    case TextureType::TEXTURE_DIFFUSE:
        textureTypename =  "texture_diffuse";
        break;
    case TextureType::TEXTURE_SPECULAR:
        textureTypename =  "texture_specular";
        break;
    case TextureType::TEXTURE_NORMAL:
        textureTypename =  "texture_normal";
        break;
    case TextureType::TEXTURE_HEIGHT:
        textureTypename =  "texture_height";
        break;
    default:
        break;
    }
    return textureTypename;
}


Texture::Texture(const std::string& path, TextureType textureType)
    :m_path(path),
    m_type(textureType)
{
    m_refCnt = new unsigned(1);
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, &m_nrChannels, 0);
    if (data)
    {
        switch (m_nrChannels)
        {
        case 3:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            break;
        case 4:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            break;
        default:
            GL_LOG_E("load texture failed. don't support nr channels %d", m_nrChannels);
            std::abort();
            break;
        }
        
        glGenerateMipmap(m_id);
    }
    else
    {
        GL_LOG_E("Failed to load texture %s", path.c_str());
        std::abort();
    }
     GL_LOG_D("load texture %s type %s witdh %d height %d nrChannels %d", path.c_str(), translateTextureTypeName(m_type).c_str(), m_width, m_height, m_nrChannels);
    stbi_image_free(data);
}

Texture::Texture(const Texture& other)
{
    *this = other;
}

Texture& Texture::operator=(const Texture& other)
{
    if (this != &other)
    {
        m_id = other.m_id;
        m_width = other.m_width;
        m_nrChannels = other.m_nrChannels;
        m_type = other.m_type;
        m_path = other.m_path;
        m_refCnt = other.m_refCnt;

        (*m_refCnt)++;
    }
    return *this;
}

Texture::Texture(Texture&& other)
{
    *this = std::move(other);
}

Texture& Texture::operator=(Texture&& other)
{
    if (this != &other)
    {
        m_id = other.m_id;
        m_width = other.m_width;
        m_nrChannels = other.m_nrChannels;
        m_type = other.m_type;
        m_path = other.m_path;
        m_refCnt = other.m_refCnt;

        other.m_refCnt = nullptr;

    }
    return *this;
}

Texture::~Texture()
{
    if (m_refCnt)
    {
        (*m_refCnt)--;
        if (*m_refCnt == 0)
        {
            GL_LOG_D("release texture %d", m_id);
            glDeleteTextures(1, &m_id);

            delete m_refCnt;
            m_refCnt = nullptr;
        }
    }

}

void Texture::setWarpType(unsigned int SWarpType, unsigned int TWarpType, const std::vector<float>& borderColor)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, SWarpType);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TWarpType);
    if (SWarpType == GL_CLAMP_TO_BORDER || TWarpType == GL_CLAMP_TO_BORDER)
    {
        assert(borderColor.size() == 4);
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &borderColor[0]);
    }
}

void Texture::setFilterType(unsigned int minFilter, unsigned int magFilter)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}