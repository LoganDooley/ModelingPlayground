#include "OpenGLUniformBlock.h"

#include <iostream>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

OpenGLUniformBlock::OpenGLUniformBlock(GLuint uniformBlockIndex, GLuint programId):
    m_programId(programId)
{
    GLint uniformBlockDataSize;
    glGetActiveUniformBlockiv(m_programId, uniformBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &uniformBlockDataSize);
    GLint uniformBlockBinding;
    glGetActiveUniformBlockiv(m_programId, uniformBlockIndex, GL_UNIFORM_BLOCK_BINDING, &uniformBlockBinding);

    glGenBuffers(1, &m_uniformBufferObject);
    if (m_uniformBufferObject == -1)
    {
        std::cout << "OpenGLShader|RegisterUniformBufferObject: Failed to create uniform buffer object!\n";
        return;
    }
    glBindBufferBase(GL_UNIFORM_BUFFER, uniformBlockBinding, m_uniformBufferObject);
    glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferObject);
    glBufferData(GL_UNIFORM_BUFFER, uniformBlockDataSize, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    GLint uniformBlockActiveUniforms;
    glGetActiveUniformBlockiv(m_programId, uniformBlockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS,
                              &uniformBlockActiveUniforms);
    std::vector<GLint> uniformBlockUniformIndices(uniformBlockActiveUniforms);
    glGetActiveUniformBlockiv(m_programId, uniformBlockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES,
                              uniformBlockUniformIndices.data());

    GLint maxUniformNameLength;
    glGetProgramiv(m_programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);

    for (int i = 0; i < uniformBlockActiveUniforms; i++)
    {
        std::vector<GLchar> uniformName(maxUniformNameLength);
        GLsizei uniformNameLength;
        GLenum uniformType;
        GLint uniformSize;
        glGetActiveUniform(m_programId, uniformBlockUniformIndices[i], maxUniformNameLength, &uniformNameLength,
                           &uniformSize, &uniformType,
                           uniformName.data());
        constexpr GLenum props[] = {GL_OFFSET};
        GLsizei length;
        GLint uniformOffset;
        glGetProgramResourceiv(m_programId, GL_UNIFORM, uniformBlockUniformIndices[i], 1, props, 1, &length,
                               &uniformOffset);
        m_uniformBlockUniformOffsets[uniformName.data()] = uniformOffset;
    }
}

void OpenGLUniformBlock::SetValue(const std::string& uniformName, int uniformValue)
{
    if (!m_uniformBlockUniformOffsets.contains(uniformName))
    {
        std::cout << "OpenGLUniformBlock|SetValue: Uniform: " << uniformName << " is not registered!\n";
        return;
    }

    glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferObject);
    glBufferSubData(GL_UNIFORM_BUFFER, m_uniformBlockUniformOffsets[uniformName], sizeof(int), &uniformValue);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OpenGLUniformBlock::SetValue(const std::string& uniformName, uint64_t uniformValue)
{
    if (!m_uniformBlockUniformOffsets.contains(uniformName))
    {
        std::cout << "OpenGLUniformBlock|SetValue: Uniform: " << uniformName << " is not registered!\n";
        return;
    }

    glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferObject);
    glBufferSubData(GL_UNIFORM_BUFFER, m_uniformBlockUniformOffsets[uniformName], sizeof(uint64_t), &uniformValue);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OpenGLUniformBlock::SetValue(const std::string& uniformName, float uniformValue)
{
    if (!m_uniformBlockUniformOffsets.contains(uniformName))
    {
        std::cout << "OpenGLUniformBlock|SetValue: Uniform: " << uniformName << " is not registered!\n";
        return;
    }

    glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferObject);
    glBufferSubData(GL_UNIFORM_BUFFER, m_uniformBlockUniformOffsets[uniformName], sizeof(float), &uniformValue);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OpenGLUniformBlock::SetValue(const std::string& uniformName, glm::mat4 uniformValue)
{
    if (!m_uniformBlockUniformOffsets.contains(uniformName))
    {
        std::cout << "OpenGLUniformBlock|SetValue: Uniform: " << uniformName << " is not registered!\n";
        return;
    }

    glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferObject);
    glBufferSubData(GL_UNIFORM_BUFFER, m_uniformBlockUniformOffsets[uniformName], sizeof(glm::mat4),
                    glm::value_ptr(uniformValue));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
