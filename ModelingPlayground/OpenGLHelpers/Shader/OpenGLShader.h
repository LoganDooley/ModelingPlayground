#pragma once
#include <memory>
#include <string>
#include <unordered_map>

#include "OpenGLUniformBlock.h"
#include "OpenGLUniformVariable.h"
#include "OpenGLShaderStorageBlock.h"
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

    template <typename T>
    void SetUniformBlockUniformValue(const std::string& uniformBlockName, const std::string& uniformBlockUniformName,
                                     const T& uniformValue)
    {
        if (!ValidateUniformBlockName(uniformBlockName))
        {
            return;
        }
        std::cout << "PLEASE IMPLEMENT ME\n";
        //m_uniformBlocks[uniformBlockName]->SetValue<T>(uniformBlockUniformName, uniformValue);
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
    std::unordered_map<std::string, std::unique_ptr<OpenGLUniformBlock>> m_uniformBlocks;
    std::unordered_map<std::string, std::unique_ptr<OpenGLShaderStorageBlock>> m_shaderStorageBlocks;
    std::vector<VertexAttribute> m_attributes;
    GLuint m_shaderProgramId;
};
