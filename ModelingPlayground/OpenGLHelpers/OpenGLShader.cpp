#include "OpenGLShader.h"

#include <iostream>
#include <ostream>

#include "../Application/ShaderLoader.h"
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
	if (m_uniformLocationCache.contains(uniformName))
	{
		std::cout << "OpenGLShader|RegisterUniformVariable: Uniform variable " << uniformName <<
			" is already registered!\n";
		return;
	}
	m_uniformLocationCache[uniformName] = glGetUniformLocation(m_shaderProgramId, uniformName.c_str());
	if (m_uniformLocationCache[uniformName] == -1)
	{
		std::cout << "OpenGLShader|RegisterUniformVariable: Uniform variable " << uniformName <<
			" does not exist in shader! Please check if this uniform exists and is used within the shader program.\n";
	}
}

void OpenGLShader::SetUniform1i(const std::string& uniformName, int uniformValue)
{
	if (!ValidateUniformName(uniformName))
	{
		return;
	}
	glUniform1i(m_uniformLocationCache[uniformName], uniformValue);
}

void OpenGLShader::SetUniform1ui(const std::string& uniformName, uint32_t uniformValue)
{
	if (!ValidateUniformName(uniformName))
	{
		return;
	}
	glUniform1ui(m_uniformLocationCache[uniformName], uniformValue);
}

void OpenGLShader::SetUniform1f(const std::string& uniformName, float uniformValue)
{
	if (!ValidateUniformName(uniformName))
	{
		return;
	}
	glUniform1f(m_uniformLocationCache[uniformName], uniformValue);
}

void OpenGLShader::SetUniform3f(const std::string& uniformName, glm::vec3 vector)
{
	if (!ValidateUniformName(uniformName))
	{
		return;
	}
	glUniform3f(m_uniformLocationCache[uniformName], vector.x, vector.y, vector.z);
}

void OpenGLShader::SetUniformMatrix3f(const std::string& uniformName, bool transpose, const glm::mat3& matrix)
{
	if (!ValidateUniformName(uniformName))
	{
		return;
	}
	glUniformMatrix3fv(m_uniformLocationCache[uniformName], 1, transpose, &matrix[0][0]);
}

void OpenGLShader::SetUniformMatrix4f(const std::string& uniformName, bool transpose, const glm::mat4& matrix)
{
	if (!ValidateUniformName(uniformName))
	{
		return;
	}
	glUniformMatrix4fv(m_uniformLocationCache[uniformName], 1, transpose, &matrix[0][0]);
}

bool OpenGLShader::ValidateUniformName(std::string uniformName)
{
	if (!m_uniformLocationCache.contains(uniformName))
	{
		std::cout << "Uniform variable " << uniformName << " is not registered!" << std::endl;
		return false;
	}
	return true;
}
