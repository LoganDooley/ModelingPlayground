#pragma once

#include <vector>

#include "glad/glad.h"

struct TextureParameterSetting
{
	GLenum m_texParameter;
	GLint m_texParameterValue;
};

class OpenGLTexture
{
public:
	OpenGLTexture(unsigned int width, unsigned int height, GLint internalFormat, GLenum format, GLenum dataType,
	              std::vector<TextureParameterSetting> textureParameterSettings, GLenum textureTarget);
	~OpenGLTexture();

	GLuint GetTextureId() const;
	GLuint64 GetTextureHandle() const;

	void MakeTextureResident();
	void MakeTextureNonResident();

	void Bind();
	void Unbind();

private:
	GLuint m_textureId;
	GLuint64 m_textureHandle;
	GLenum m_textureTarget;
};
