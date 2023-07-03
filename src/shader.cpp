#include "shader.h"
#include "log.h"
#include <fstream>
#include <sstream>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format off

ShaderProgram::Shader::Shader(const std::string& path, ShaderType type)
    :m_type(type)
{
    std::ifstream     ifs(path);
    std::stringstream ss;
    ss << ifs.rdbuf();

    switch (type)
    {
    case VERTEX_SHADER:
        m_id = glCreateShader(GL_VERTEX_SHADER);
        break;
    case FRAGMENT_SHADER:
        m_id = glCreateShader(GL_FRAGMENT_SHADER);
        break;
    case GEOMETRY_SHADER:
        m_id = glCreateShader(GL_GEOMETRY_SHADER);
        break;     
    default:
        GL_LOG_E("you must set shaderType, now is %d", type);
        std::abort();   
    }
    std::string sourceStr = ss.str();
    const char* shaderSource = sourceStr.c_str();
    glShaderSource(m_id, 1, &shaderSource, nullptr);
    glCompileShader(m_id);
    if (!checkError())
    {
        GL_LOG_E("complie shader error. path %s type %d", path.c_str(), type);
        std::abort();
    }
}

bool ShaderProgram::Shader::checkError()
{
    int success;
    char infoLog[512];
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(m_id, 512, NULL, infoLog);
        GL_LOG_E("gl error : %s", infoLog);
        return false;
    }
    return true;
}

ShaderProgram::Shader::~Shader()
{
    GL_LOG_D("delete shader. type %d id %d", m_type, m_id);
    glDeleteShader(m_id);
}

// ------------------ ShaderProgram ------------------

ShaderProgram::ShaderProgram(const std::string& vertextPath, const std::string& fragmentPath)
    :m_id(glCreateProgram())
{
    Shader vertextShader(vertextPath, ShaderType::VERTEX_SHADER);
    Shader fragmentShader(fragmentPath, ShaderType::FRAGMENT_SHADER);
    linkShader(vertextShader.id(), fragmentShader.id());
}

ShaderProgram::ShaderProgram(const std::string& vertextPath,
                const std::string& fragmentPath,
                const std::string& geometryPath)
{
    Shader vertextShader(vertextPath, ShaderType::VERTEX_SHADER);
    Shader fragmentShader(fragmentPath, ShaderType::FRAGMENT_SHADER);
    Shader geometryShader(geometryPath, ShaderType::GEOMETRY_SHADER);
    linkShader(vertextShader.id(), fragmentShader.id(), geometryShader.id());
}

ShaderProgram::~ShaderProgram()
{
    GL_LOG_D("release shader program %d", m_id);
    glDeleteProgram(m_id);
}

void ShaderProgram::linkShader(unsigned int vertexId, unsigned int fragmentId)
{
    glAttachShader(m_id, vertexId);
    glAttachShader(m_id, fragmentId);
    glLinkProgram(m_id);
    if (!checkError())
    {
        GL_LOG_E("link shader error");
        std::abort();
    }
}

void ShaderProgram::linkShader(unsigned int vertexId, unsigned int fragmentId, unsigned int geometryId)
{
    glAttachShader(m_id, vertexId);
    glAttachShader(m_id, fragmentId);
    glAttachShader(m_id, geometryId);
    glLinkProgram(m_id);
    if (!checkError())
    {
        GL_LOG_E("link shader error");
        std::abort();
    }
}

void ShaderProgram::use()
{
    glUseProgram(m_id);
}

// ----------------- uniform util function -----------------
void ShaderProgram::setBool(const std::string& name, bool value) const
{
    setInt(name, static_cast<int>(value));
}

void ShaderProgram::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

void ShaderProgram::setMat4(const std::string& name, const float* value) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, value);
}

void ShaderProgram::setVec3(const std::string& name, const float* value) const
{
    glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, value); 
}

bool ShaderProgram::checkError()
{
    int success;
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(m_id, 512, nullptr, infoLog);
        return false;
    }
    return true;
}