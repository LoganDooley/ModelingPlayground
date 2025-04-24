#include "OpenGLShader.h"

#include <iostream>
#include <ostream>
#include <utility>

#include "OpenGLUniformBlock.h"
#include "../../Application/ShaderLoader.h"
#include "glm/ext.hpp"
#include "glm/glm.hpp"

OpenGLShader::OpenGLShader()
{
}

OpenGLShader::~OpenGLShader()
{
    if (m_shaderProgramId > 0)
    {
        glDeleteProgram(m_shaderProgramId);
    }
}

void OpenGLShader::LoadShader(const char* vertexFilePath, const char* fragmentFilePath)
{
    m_shaderProgramId = ShaderLoader::createShaderProgram(vertexFilePath, fragmentFilePath);

    RegisterProgramUniformsAndBlocks();
}

void OpenGLShader::LoadShader(const char* vertexFilePath, const char* geometryFilePath, const char* fragmentFilePath)
{
    m_shaderProgramId = ShaderLoader::createShaderProgram(vertexFilePath, geometryFilePath, fragmentFilePath);

    RegisterProgramUniformsAndBlocks();
}

void OpenGLShader::BindShader() const
{
    glUseProgram(m_shaderProgramId);
}

void OpenGLShader::UnbindShader() const
{
    glUseProgram(0);
}

void OpenGLShader::RegisterProgramUniformsAndBlocks()
{
    GLint uniformBlockCount;
    glGetProgramiv(m_shaderProgramId, GL_ACTIVE_UNIFORM_BLOCKS, &uniformBlockCount);

    GLint maxUniformBlockNameLength;
    glGetProgramiv(m_shaderProgramId, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &maxUniformBlockNameLength);

    for (GLuint i = 0; i < uniformBlockCount; i++)
    {
        GLint uniformBlockNameLength;
        std::vector<GLchar> uniformBlockName(maxUniformBlockNameLength);
        glGetActiveUniformBlockName(m_shaderProgramId, i, maxUniformBlockNameLength, &uniformBlockNameLength,
                                    uniformBlockName.data());

        RegisterUniformBlock(uniformBlockName.data(), i);
    }


    GLint uniformCount;
    glGetProgramiv(m_shaderProgramId, GL_ACTIVE_UNIFORMS, &uniformCount);

    GLint maxUniformNameLength;
    glGetProgramiv(m_shaderProgramId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);

    for (GLuint i = 0; i < uniformCount; i++)
    {
        std::vector<GLchar> uniformName(maxUniformNameLength);
        GLsizei uniformNameLength;
        GLenum uniformType;
        GLint uniformSize;
        glGetActiveUniform(m_shaderProgramId, i, maxUniformNameLength, &uniformNameLength, &uniformSize, &uniformType,
                           uniformName.data());
        RegisterUniformVariable(uniformName.data());
    }
}

void OpenGLShader::RegisterUniformVariable(const std::string& uniformName)
{
    if (m_uniforms.contains(uniformName) || glGetUniformLocation(m_shaderProgramId, uniformName.c_str()) == -1)
    {
        return;
    }
    m_uniforms[uniformName] = std::make_unique<OpenGLUniformVariable>(uniformName, m_shaderProgramId);
}


void OpenGLShader::RegisterUniformBlock(std::string uniformBlockName, GLuint uniformBlockIndex)
{
    if (m_uniformBlocks.contains(uniformBlockName))
    {
        std::cout << "OpenGLShader|RegisterUniformBufferObject: Uniform buffer object " << uniformBlockName <<
            "	is already registered!\n";
        return;
    }

    m_uniformBlocks[uniformBlockName] = std::make_unique<OpenGLUniformBlock>(
        uniformBlockIndex, m_shaderProgramId);
}

bool OpenGLShader::ValidateUniformName(const std::string& uniformName) const
{
    if (!m_uniforms.contains(uniformName))
    {
        std::cout << "Uniform variable " << uniformName << " is not registered!" << std::endl;
        return false;
    }
    return true;
}

bool OpenGLShader::ValidateUniformBlockName(const std::string& uniformBlockName) const
{
    if (!m_uniformBlocks.contains(uniformBlockName))
    {
        std::cout << "Uniform buffer object " << uniformBlockName << " is not registered!" << std::endl;
        return false;
    }
    return true;
}
