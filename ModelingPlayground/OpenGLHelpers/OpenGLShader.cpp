#include "OpenGLShader.h"

#include <iostream>
#include <ostream>

#include "glm/glm.hpp"
#include "../Application/ShaderLoader.h"

OpenGLShader::OpenGLShader(const char* vertexFilePath, const char* fragmentFilePath)
{
    m_shaderID = ShaderLoader::createShaderProgram(vertexFilePath, fragmentFilePath);
}

void OpenGLShader::BindShader() const
{
    glUseProgram(m_shaderID);
}

void OpenGLShader::UnbindShader() const
{
    glUseProgram(0);
}

void OpenGLShader::RegisterUniformVariable(const std::string& uniformName)
{
    if (m_uniformLocationCache.contains(uniformName))
    {
        std::cout << "Unifom variable " << uniformName << " is already registered!" << std::endl;
        return;
    }
    m_uniformLocationCache[uniformName] = glGetUniformLocation(m_shaderID, uniformName.c_str());
}

void OpenGLShader::SetUniform1ui(const std::string& uniformName, uint32_t uniformValue)
{
    if (!ValidateUniformName(uniformName))
    {
        return;
    }
    glUniform1ui(m_uniformLocationCache[uniformName], uniformValue);
}

void OpenGLShader::SetUniformMatrix3f(const std::string& uniformName, bool transpose, const glm::mat3& matrix)
{
    if (!ValidateUniformName(uniformName))
    {
        return;
    }
    glUniformMatrix3fv(m_uniformLocationCache[uniformName], 1, transpose, &matrix[0][0]);
}

void OpenGLShader::SetUniformMatrix4f(const std::string& uniformName, bool transpose, const glm::mat4& matrix)
{
    if (!ValidateUniformName(uniformName))
    {
        return;
    }
    glUniformMatrix4fv(m_uniformLocationCache[uniformName], 1, transpose, &matrix[0][0]);
}

void OpenGLShader::SetUniform3f(const std::string& uniformName, glm::vec3 vector)
{
    if (!ValidateUniformName(uniformName))
    {
        return;
    }
    glUniform3f(m_uniformLocationCache[uniformName], vector.x, vector.y, vector.z);
}

bool OpenGLShader::ValidateUniformName(std::string uniformName)
{
    if (!m_uniformLocationCache.contains(uniformName))
    {
        std::cout << "Uniform variable " << uniformName << " is not registered!" << std::endl;
        return false;
    }
    return true;
}
