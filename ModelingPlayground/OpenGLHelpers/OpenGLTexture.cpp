#include "OpenGLTexture.h"

OpenGLTexture::OpenGLTexture(unsigned int width, unsigned int height, GLint internalFormat, GLenum format,
                             GLenum dataType, std::vector<TextureParameterSetting> textureParameterSettings,
                             GLenum textureTarget):
	m_textureId(0),
	m_textureHandle(0),
	m_textureTarget(textureTarget)
{
	glGenTextures(1, &m_textureId);
	Bind();

	if (m_textureTarget == GL_TEXTURE_2D)
	{
		glTexImage2D(m_textureTarget, 0, internalFormat, width, height, 0, format, dataType, nullptr);

		for (const auto& textureParameterSetting : textureParameterSettings)
		{
			glTexParameteri(m_textureTarget, textureParameterSetting.m_texParameter,
			                textureParameterSetting.m_texParameterValue);
		}
	}
	else if (m_textureTarget == GL_TEXTURE_CUBE_MAP)
	{
		for (int i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, dataType,
			             nullptr);
		}

		for (const auto& textureParameterSetting : textureParameterSettings)
		{
			glTexParameteri(m_textureTarget, textureParameterSetting.m_texParameter,
			                textureParameterSetting.m_texParameterValue);
		}
	}
	Unbind();

	m_textureHandle = glGetTextureHandleARB(m_textureId);
}

OpenGLTexture::~OpenGLTexture()
{
	glDeleteTextures(1, &m_textureId);
}

GLuint OpenGLTexture::GetTextureId() const
{
	return m_textureId;
}

GLuint64 OpenGLTexture::GetTextureHandle() const
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
