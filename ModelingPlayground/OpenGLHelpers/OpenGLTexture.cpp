#include "OpenGLTexture.h"

OpenGLTexture::OpenGLTexture(unsigned int width, unsigned int height, GLint internalFormat, GLenum format,
                             GLenum dataType, std::vector<TextureParameterSetting> textureParameterSettings,
                             GLenum textureTarget):
	m_textureTarget(textureTarget)
{
	glGenTextures(1, &m_textureId);
	Bind();
	glTexImage2D(m_textureTarget, 0, internalFormat, width, height, 0, format, dataType, nullptr);

	for (const auto& textureParameterSetting : textureParameterSettings)
	{
		glTexParameteri(m_textureTarget, textureParameterSetting.m_texParameter,
		                textureParameterSetting.m_texParameterValue);
	}
	Unbind();

	m_textureHandle = glGetTextureHandleARB(m_textureId);
}

OpenGLTexture::~OpenGLTexture()
{
	glDeleteTextures(1, &m_textureId);
}

GLuint OpenGLTexture::GetTextureId()
{
	return m_textureId;
}

GLuint64 OpenGLTexture::GetTextureHandle()
{
	return m_textureHandle;
}

void OpenGLTexture::MakeTextureResident()
{
	glMakeTextureHandleResidentARB(m_textureHandle);
}

void OpenGLTexture::MakeTextureNonResident()
{
	glMakeTextureHandleNonResidentARB(m_textureHandle);
}

void OpenGLTexture::Bind()
{
	glBindTexture(m_textureTarget, m_textureId);
}

void OpenGLTexture::Unbind()
{
	glBindTexture(m_textureTarget, 0);
}
