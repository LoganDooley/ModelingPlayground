#pragma once

#include <string>
#include <glm/fwd.hpp>

#include "glad/glad.h"

class OpenGLUniformVariable
{
public:
	OpenGLUniformVariable(std::string uniformName, GLuint programId);

	void SetValue(int uniformValue) const;
	void SetValue(unsigned int uniformValue) const;
	void SetValue(float uniformValue) const;
	void SetValue(glm::vec3 vector) const;
	void SetValue(bool transpose, const glm::mat3& matrix) const;
	void SetValue(bool transpose, const glm::mat4& matrix) const;

private:
	GLint m_uniformLocation;
};
