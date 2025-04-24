#pragma once

#include <string>
#include <unordered_map>
#include <glm/mat4x4.hpp>

#include "glad/glad.h"

class OpenGLUniformBlock
{
public:
    OpenGLUniformBlock(GLuint uniformBlockIndex, GLuint programId);

    void SetValue(const std::string& uniformName, int uniformValue);
    void SetValue(const std::string& uniformName, uint64_t uniformValue);
    void SetValue(const std::string& uniformName, float uniformValue);
    void SetValue(const std::string& uniformName, glm::mat4 uniformValue);

private:
    GLuint m_programId;
    GLuint m_uniformBufferObject;
    std::unordered_map<std::string, GLint> m_uniformBlockUniformOffsets;
};
