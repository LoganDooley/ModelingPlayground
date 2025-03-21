#pragma once
#include <string>
#include <unordered_map>
#include <glm/fwd.hpp>

#include "glad/glad.h"

class OpenGLShader
{
public:
    OpenGLShader();

    void LoadShader(const char * vertexFilePath, const char * fragmentFilePath);
    void BindShader() const;
    void UnbindShader() const;
    
    void RegisterUniformVariable(const std::string& uniformName);

    void SetUniform1i(const std::string& uniformName, int uniformValue);
    void SetUniform1ui(const std::string& uniformName, uint32_t uniformValue);
    void SetUniform1f(const std::string& uniformName, float uniformValue);
    void SetUniform3f(const std::string& uniformName, glm::vec3 vector);
    void SetUniformMatrix3f(const std::string& uniformName, bool transpose, const glm::mat3& matrix);
    void SetUniformMatrix4f(const std::string& uniformName, bool transpose, const glm::mat4& matrix);

private:
    bool ValidateUniformName(std::string uniformName);
    
    std::unordered_map<std::string, GLint> m_uniformLocationCache;
    GLuint m_shaderID;
};
