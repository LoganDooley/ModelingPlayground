#pragma once

#include "../../Application/Rendering/Wrappers/OpenGLBuffer.h"

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "glad/glad.h"

class OpenGLUniformBlock
{
public:
    OpenGLUniformBlock(GLuint uniformBlockIndex, GLuint programId);
    ~OpenGLUniformBlock() = default;

    GLuint GetUniformBlockDataSize() const;
    GLuint GetUniformBlockBinding() const;

    template <typename T>
    void SetValue(const std::shared_ptr<OpenGLBuffer>& uniformBufferObject, const std::string& uniformName,
                  const T& uniformValue)
    {
        if (!m_uniformBlockUniformOffsets.contains(uniformName))
        {
            std::cout << "OpenGLUniformBlock|SetValue: Uniform: " << uniformName << " is not registered!\n";
            return;
        }

        uniformBufferObject->SetSubData(m_uniformBlockUniformOffsets[uniformName], uniformValue);
    }

private:
    GLuint m_programId;
    GLuint m_uniformBlockDataSize;
    GLuint m_uniformBlockBinding;
    std::unordered_map<std::string, GLint> m_uniformBlockUniformOffsets;
};
