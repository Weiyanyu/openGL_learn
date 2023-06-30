#pragma once
#include <string>

class Texture
{
public:
    Texture(const std::string& path);

public:
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