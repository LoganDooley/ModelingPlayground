#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <glm/fwd.hpp>

#include "OpenGLUniformVariable.h"
#include "glad/glad.h"

class OpenGLShader
{
public:
	OpenGLShader();
	~OpenGLShader();

	void LoadShader(const char* vertexFilePath, const char* fragmentFilePath);
	void BindShader() const;
	void UnbindShader() const;

	void RegisterUniformVariable(const std::string& uniformName);
	bool RegisterUniformBufferObject(std::string uniformBufferObjectName, GLsizeiptr sizeInBytes,
	                                 GLuint bindingLocation);

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

	void SetUniformBufferObjectSubData(const std::string& uniformBufferObjectName, GLintptr offset,
	                                   int data) const;

	void SetUniformBufferObjectSubData(const std::string& uniformBufferObjectName, GLintptr offset,
	                                   uint64_t data) const;

	void SetUniformBufferObjectSubData(const std::string& uniformBufferObjectName, GLintptr offset,
	                                   glm::vec3 data) const;

	void SetUniformBufferObjectSubData(const std::string& uniformBufferObjectName, GLintptr offset,
	                                   glm::mat4 data) const;

private:
	bool ValidateUniformName(const std::string& uniformName) const;
	bool ValidateUniformBufferObjectName(const std::string& uniformBufferObjectName) const;

	std::unordered_map<std::string, std::unique_ptr<OpenGLUniformVariable>> m_uniforms;
	std::unordered_map<std::string, std::pair<GLuint, GLuint>> m_uniformBufferObjects;
	GLuint m_shaderProgramId;
};
