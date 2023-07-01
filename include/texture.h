#pragma once
#include <string>
#include <vector>

class Texture
{
public:
    Texture(const std::string& path);
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    ~Texture();

public:
    void setWarpType(unsigned int SWarpType, unsigned int TWarpType, const std::vector<float>& borderColor = std::vector<float>());
    void setFilterType(unsigned int minFilter, unsigned int magFilter);

    unsigned int id() const
    {
        return m_id;
    }

private:
    int m_width;
    int m_height;
    int m_nrChannels;

    unsigned int m_id;
};