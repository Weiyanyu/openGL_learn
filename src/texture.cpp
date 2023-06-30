#include "texture.h"
#include "log.h"
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format off

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string& path)
{

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, &m_nrChannels, 0);
    if (data)
    {
        GL_LOG_D("load texture %s witdh %d height %d nrChannels %d", path.c_str(), m_width, m_height, m_nrChannels);
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
    stbi_image_free(data);


}

void Texture::setWarpType(unsigned int SWarpType, unsigned int TWarpType, const std::vector<float>& borderColor)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, SWarpType);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TWarpType);
    if (SWarpType == GL_CLAMP_TO_BORDER || TWarpType == GL_CLAMP_TO_BORDER)
    {
        assert(borderColor.size() == 4);
        // temp
        float color[4];
        std::memcpy(color, &borderColor[0], 4);
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
    }
}

void Texture::setFilterType(unsigned int minFilter, unsigned int magFilter)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}