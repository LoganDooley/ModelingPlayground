#include "OpenGLUniformBlock.h"

#include "../../Application/Rendering/Wrappers/OpenGLBuffer.h"

#include <iostream>
#include <vector>

OpenGLUniformBlock::OpenGLUniformBlock(GLuint uniformBlockIndex, GLuint programId):
    m_programId(programId)
{
    GLint uniformBlockDataSize;
    glGetActiveUniformBlockiv(m_programId, uniformBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &uniformBlockDataSize);
    m_uniformBlockDataSize = uniformBlockDataSize;
    GLint uniformBlockBinding;
    glGetActiveUniformBlockiv(m_programId, uniformBlockIndex, GL_UNIFORM_BLOCK_BINDING, &uniformBlockBinding);
    m_uniformBlockBinding = uniformBlockBinding;

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

GLuint OpenGLUniformBlock::GetUniformBlockDataSize() const
{
    return m_uniformBlockDataSize;
}

GLuint OpenGLUniformBlock::GetUniformBlockBinding() const
{
    return m_uniformBlockBinding;
}
