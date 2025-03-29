#pragma once

#include "glad/glad.h"

class OpenGLRenderbuffer
{
public:
	OpenGLRenderbuffer(unsigned int width, unsigned int height, GLenum internalFormat);
	~OpenGLRenderbuffer();

	GLuint GetRenderbufferId();

	void Bind();
	void Unbind();

private:
	GLuint m_renderbufferId;
};
