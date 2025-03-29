#include "OpenGLRenderbuffer.h"

OpenGLRenderbuffer::OpenGLRenderbuffer(unsigned int width, unsigned int height, GLenum internalFormat)
{
	glGenRenderbuffers(1, &m_renderbufferId);
	Bind();
	glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
	Unbind();
}

OpenGLRenderbuffer::~OpenGLRenderbuffer()
{
	glDeleteRenderbuffers(1, &m_renderbufferId);
}

GLuint OpenGLRenderbuffer::GetRenderbufferId()
{
	return m_renderbufferId;
}

void OpenGLRenderbuffer::Bind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, m_renderbufferId);
}

void OpenGLRenderbuffer::Unbind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
