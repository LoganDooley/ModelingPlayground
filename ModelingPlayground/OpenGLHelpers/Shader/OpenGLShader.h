#pragma once
#include <memory>
#include <string>
#include <unordered_map>

#include "OpenGLUniformVariable.h"
#include "OpenGLShaderBlock.h"
#include "../../Application/Rendering/Primitives/Primitive.h"
#include "glad/glad.h"

class OpenGLShader
{
public:
    OpenGLShader();
    ~OpenGLShader();

    void LoadShader(const char* vertexFilePath, const char* fragmentFilePath);
    void LoadShader(const char* vertexFilePath, const char* geometryFilePath, const char* fragmentFilePath);
    void BindShader() const;
    void UnbindShader() const;

    const std::shared_ptr<OpenGLShaderBlock>& GetUniformBlock(const std::string& name) const;
    const std::shared_ptr<OpenGLShaderBlock>& GetShaderStorageBlock(const std::string& name) const;

    template <typename T>
    void SetUniform(const std::string& uniformName, T uniformValue)
    {
        if (!ValidateUniformName(uniformName))
        {
            return;
        }
        m_uniforms[uniformName]->SetValue(uniformValue);
    }

    template <typename T>
    void SetUniform(const std::string& uniformName, bool transpose, const T& matrix)
    {
        if (!ValidateUniformName(uniformName))
        {
            return;
        }
        m_uniforms[uniformName]->SetValue(transpose, matrix);
    }

private:
    void RegisterProgramUniformsAndBlocks();
    void RegisterProgramAttributes();
    void RegisterUniformVariable(const std::string& uniformName);
    void RegisterUniformBlock(std::string uniformBlockName, GLuint uniformBlockIndex);
    void RegisterShaderStorageBlock(std::string shaderStorageBlockName, GLuint shaderStorageBlockIndex);
    void RegisterAttribute(const std::string& attributeName, GLenum attributeType);
    bool ValidateUniformName(const std::string& uniformName) const;
    bool ValidateUniformBlockName(const std::string& uniformBlockName) const;

    std::unordered_map<std::string, std::unique_ptr<OpenGLUniformVariable>> m_uniforms;
    std::unordered_map<std::string, std::shared_ptr<OpenGLShaderBlock>> m_uniformBlocks;
    std::unordered_map<std::string, std::shared_ptr<OpenGLShaderBlock>> m_shaderStorageBlocks;
    std::vector<VertexAttribute> m_attributes;
    GLuint m_shaderProgramId;
};
