#include "OpenGLTexture.h"

OpenGLTexture::OpenGLTexture(unsigned int width, unsigned int height, GLint internalFormat, GLenum format,
                             GLenum dataType, GLenum minFilter, GLenum magFilter, GLenum textureTarget):
	m_textureTarget(textureTarget)
{
	glGenTextures(1, &m_textureId);
	Bind();
	glTexImage2D(m_textureTarget, 0, internalFormat, width, height, 0, format, dataType, nullptr);
	glTexParameteri(m_textureTarget, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(m_textureTarget, GL_TEXTURE_MAG_FILTER, magFilter);
	Unbind();
}

OpenGLTexture::~OpenGLTexture()
{
	glDeleteTextures(1, &m_textureId);
}

GLuint OpenGLTexture::GetTextureId()
{
	return m_textureId;
}

void OpenGLTexture::Bind()
{
	glBindTexture(m_textureTarget, m_textureId);
}

void OpenGLTexture::Unbind()
{
	glBindTexture(m_textureTarget, 0);
}
