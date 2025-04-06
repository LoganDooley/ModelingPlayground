#include "OpenGLShader.h"

#include <iostream>
#include <ostream>
#include <utility>

#include "../Application/ShaderLoader.h"
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
}

void OpenGLShader::LoadShader(const char* vertexFilePath, const char* geometryFilePath, const char* fragmentFilePath)
{
	m_shaderProgramId = ShaderLoader::createShaderProgram(vertexFilePath, geometryFilePath, fragmentFilePath);
}

void OpenGLShader::BindShader() const
{
	glUseProgram(m_shaderProgramId);
}

void OpenGLShader::UnbindShader() const
{
	glUseProgram(0);
}

void OpenGLShader::RegisterUniformVariable(const std::string& uniformName)
{
	if (m_uniforms.contains(uniformName))
	{
		std::cout << "OpenGLShader|RegisterUniformVariable: Uniform variable " << uniformName <<
			" is already registered!\n";
		return;
	}
	m_uniforms[uniformName] = std::make_unique<OpenGLUniformVariable>(uniformName, m_shaderProgramId);
}

bool OpenGLShader::RegisterUniformBufferObject(std::string uniformBufferObjectName, GLsizeiptr sizeInBytes,
                                               GLuint bindingLocation)
{
	if (m_uniformBufferObjects.contains(uniformBufferObjectName))
	{
		std::cout << "OpenGLShader|RegisterUniformBufferObject: Uniform buffer object " << uniformBufferObjectName <<
			"	is already registered!\n";
		return false;
	}

	GLuint uniformBufferObject = 0;
	glGenBuffers(1, &uniformBufferObject);
	if (uniformBufferObject == -1)
	{
		std::cout << "OpenGLShader|RegisterUniformBufferObject: Failed to create uniform buffer object!\n";
		return false;
	}
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingLocation, uniformBufferObject);
	m_uniformBufferObjects[uniformBufferObjectName] = {uniformBufferObject, bindingLocation};
	glBindBuffer(GL_UNIFORM_BUFFER, uniformBufferObject);
	glBufferData(GL_UNIFORM_BUFFER, sizeInBytes, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	return true;
}

void OpenGLShader::SetUniformBufferObjectSubData(const std::string& uniformBufferObjectName, GLintptr offset,
                                                 int data) const
{
	if (!ValidateUniformBufferObjectName(uniformBufferObjectName))
	{
		return;
	}
	std::vector<int> intData = {data};
	glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferObjects.at(uniformBufferObjectName).first);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(int), intData.data());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OpenGLShader::SetUniformBufferObjectSubData(const std::string& uniformBufferObjectName, GLintptr offset,
                                                 uint64_t data) const
{
	if (!ValidateUniformBufferObjectName(uniformBufferObjectName))
	{
		return;
	}
	std::vector<uint64_t> intData = {data};
	glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferObjects.at(uniformBufferObjectName).first);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(uint64_t), intData.data());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OpenGLShader::SetUniformBufferObjectSubData(const std::string& uniformBufferObjectName, GLintptr offset,
                                                 float data) const
{
	if (!ValidateUniformBufferObjectName(uniformBufferObjectName))
	{
		return;
	}
	std::vector<float> intData = {data};
	glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferObjects.at(uniformBufferObjectName).first);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), intData.data());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OpenGLShader::SetUniformBufferObjectSubData(const std::string& uniformBufferObjectName, GLintptr offset,
                                                 glm::vec2 data) const
{
	if (!ValidateUniformBufferObjectName(uniformBufferObjectName))
	{
		return;
	}
	std::vector<float> floatData = {data.x, data.y};
	glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferObjects.at(uniformBufferObjectName).first);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float) * 2, floatData.data());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OpenGLShader::SetUniformBufferObjectSubData(const std::string& uniformBufferObjectName, GLintptr offset,
                                                 glm::vec3 data) const
{
	if (!ValidateUniformBufferObjectName(uniformBufferObjectName))
	{
		return;
	}
	std::vector<float> floatData = {data.x, data.y, data.z};
	glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferObjects.at(uniformBufferObjectName).first);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float) * 3, floatData.data());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OpenGLShader::SetUniformBufferObjectSubData(const std::string& uniformBufferObjectName, GLintptr offset,
                                                 glm::mat4 data) const
{
	if (!ValidateUniformBufferObjectName(uniformBufferObjectName))
	{
		return;
	}

	glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferObjects.at(uniformBufferObjectName).first);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::mat4), value_ptr(data));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
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

bool OpenGLShader::ValidateUniformBufferObjectName(const std::string& uniformBufferObjectName) const
{
	if (!m_uniformBufferObjects.contains(uniformBufferObjectName))
	{
		std::cout << "Uniform buffer object " << uniformBufferObjectName << " is not registered!" << std::endl;
		return false;
	}
	return true;
}
