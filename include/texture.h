#pragma once
#include <string>
#include <vector>

enum TextureType
{
    TEXTURE_BUFFER = 0,
    TEXTURE_DIFFUSE,
    TEXTURE_SPECULAR,
    TEXTURE_NORMAL,
    TEXTURE_HEIGHT,
};

class Texture
{
public:
    Texture(const std::string& path, TextureType textureType = TextureType::TEXTURE_DIFFUSE, bool isFlip = true);
    Texture(int width, int height, int nrChannels);
    Texture(const Texture&);
    Texture& operator=(const Texture&);
    Texture(Texture&&);
    Texture& operator=(Texture&&);
    ~Texture();

public:
    static std::string translateTextureTypeName(TextureType textureType);

public:
    void setWarpType(unsigned int SWarpType, unsigned int TWarpType, const std::vector<float>& borderColor = std::vector<float>());
    void setFilterType(unsigned int minFilter, unsigned int magFilter);

    unsigned int id() const
    {
        return m_id;
    }

    TextureType type() const
    {
        return m_type;
    }

    std::string path() const
    {
        return m_path;
    }

private:
    int           m_width;
    int           m_height;
    int           m_nrChannels;
    unsigned int  m_id;
    TextureType   m_type;
    std::string   m_path;
    unsigned int* m_refCnt = nullptr;
};