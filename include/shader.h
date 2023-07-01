#pragma once

#include <string>

enum ShaderType
{
    VERTEX_SHADER   = 0,
    FRAGMENT_SHADER = 1,
    GEOMETRY_SHADER = 2,
};

class ShaderProgram
{
public:
    class Shader
    {
    public:
        Shader(const std::string& path, ShaderType type);
        unsigned int id() const
        {
            return m_id;
        }
        ~Shader();

    private:
        bool checkError();

    private:
        ShaderType   m_type;
        unsigned int m_id;
    };

public:
    ShaderProgram(const std::string& vertextPath, const std::string& fragmentPath);
    ShaderProgram(const std::string& vertextPath, const std::string& fragmentPath, const std::string& geometryPath);
    ~ShaderProgram();

    unsigned int id() const
    {
        return m_id;
    }
    void use();

    // uniform util function
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const float* value) const;

private:
    void linkShader(unsigned int vertexId, unsigned int fragmentId);
    void linkShader(unsigned int vertexId, unsigned int fragmentId, unsigned int geometryId);

    bool checkError();

private:
    unsigned int m_id;
};