#pragma once

#include "glad/glad.h"

class OpenGLTexture
{
public:
	OpenGLTexture(unsigned int width, unsigned int height, GLint internalFormat, GLenum format, GLenum dataType,
	              GLenum minFilter, GLenum magFilter, GLenum textureTarget);
	~OpenGLTexture();

	GLuint GetTextureId();

	void Bind();
	void Unbind();

private:
	GLuint m_textureId;
	GLenum m_textureTarget;
};
