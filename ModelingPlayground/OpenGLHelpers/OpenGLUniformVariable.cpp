#include "OpenGLUniformVariable.h"

#include <iostream>
#include <glm/glm.hpp>

OpenGLUniformVariable::OpenGLUniformVariable(std::string uniformName, GLuint programId):
	m_programId(programId)
{
	m_uniformLocation = glGetUniformLocation(programId, uniformName.c_str());
	if (m_uniformLocation == -1)
	{
		std::cout << "OpenGLUniformVariable: Uniform variable " << uniformName <<
			" does not exist in shader! Please check if this uniform exists and is used within the shader program.\n";
	}
}

void OpenGLUniformVariable::SetValue(int uniformValue) const
{
	glProgramUniform1i(m_programId, m_uniformLocation, uniformValue);
}

void OpenGLUniformVariable::SetValue(unsigned int uniformValue) const
{
	glProgramUniform1ui(m_programId, m_uniformLocation, uniformValue);
}

void OpenGLUniformVariable::SetValue(float uniformValue) const
{
	glProgramUniform1f(m_programId, m_uniformLocation, uniformValue);
}

void OpenGLUniformVariable::SetValue(glm::vec3 vector) const
{
	glProgramUniform3f(m_programId, m_uniformLocation, vector.x, vector.y, vector.z);
}

void OpenGLUniformVariable::SetValue(glm::vec4 vector) const
{
	glProgramUniform4f(m_programId, m_uniformLocation, vector.x, vector.y, vector.z, vector.w);
}

void OpenGLUniformVariable::SetValue(bool transpose, const glm::mat3& matrix) const
{
	glProgramUniformMatrix3fv(m_programId, m_uniformLocation, 1, transpose, &matrix[0][0]);
}

void OpenGLUniformVariable::SetValue(bool transpose, const glm::mat4& matrix) const
{
	glProgramUniformMatrix4fv(m_programId, m_uniformLocation, 1, transpose, &matrix[0][0]);
}
