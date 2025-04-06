#include "OpenGLFramebuffer.h"

#include <iostream>

#include "OpenGLRenderbuffer.h"
#include "OpenGLTexture.h"

OpenGLFramebuffer::OpenGLFramebuffer(unsigned int width, unsigned int height,
                                     const std::vector<TextureAttachmentArguments>& textureAttachments,
                                     const std::vector<RenderbufferAttachmentArguments>& renderbufferAttachments):
	m_width(width),
	m_height(height),
	m_textureAttachmentArguments(textureAttachments),
	m_renderbufferAttachmentArguments(renderbufferAttachments)
{
	CreateFramebuffer();
}

OpenGLFramebuffer::~OpenGLFramebuffer()
{
	DeleteFramebuffer();
}

void OpenGLFramebuffer::Resize(unsigned int width, unsigned int height)
{
	if (width == m_width && height == m_height)
	{
		return;
	}
	m_width = width;
	m_height = height;

	DeleteFramebuffer();
	CreateFramebuffer();
}

void OpenGLFramebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferId);
}

void OpenGLFramebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint OpenGLFramebuffer::GetFramebufferId() const
{
	return m_framebufferId;
}

std::shared_ptr<OpenGLTexture> OpenGLFramebuffer::GetTexture(GLenum attachment)
{
	if (m_textures.contains(attachment))
	{
		return m_textures[attachment];
	}
	return nullptr;
}

void OpenGLFramebuffer::DepthToRGB(GLuint* targetTexture, int& width, int& height)
{
	Bind();
	std::vector<GLfloat> depthData(m_width * m_height);
	glReadPixels(0, 0, m_width, m_height, GL_DEPTH_COMPONENT, GL_FLOAT, depthData.data());
	std::vector<unsigned char> pixels(4 * m_width * m_height, 0);
	for (int i = 0; i < m_width * m_height; i++)
	{
		pixels[4 * i] = depthData[i] * 255;
		if (pixels[4 * i] != 255)
		{
			unsigned char pixel = pixels[4 * i];
		}
		pixels[4 * i + 3] = 255;
	}
	glDeleteTextures(1, targetTexture);
	glGenTextures(1, targetTexture);
	glBindTexture(GL_TEXTURE_2D, *targetTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
	glBindTexture(GL_TEXTURE_2D, 0);
	Unbind();
	width = m_width;
	height = m_height;
}

void OpenGLFramebuffer::DeleteFramebuffer()
{
	glDeleteFramebuffers(1, &m_framebufferId);
	m_textures.clear();
	m_renderbuffers.clear();
}

void OpenGLFramebuffer::CreateFramebuffer()
{
	// Create framebuffer
	glGenFramebuffers(1, &m_framebufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferId);

	// Create and bind texture attachments
	for (unsigned int i = 0; i < m_textureAttachmentArguments.size(); i++)
	{
		TextureAttachmentArguments textureAttachmentArguments = m_textureAttachmentArguments[i];
		m_textures[textureAttachmentArguments.m_attachment] = std::make_shared<OpenGLTexture>(m_width, m_height,
			textureAttachmentArguments.m_internalFormat,
			textureAttachmentArguments.m_format,
			textureAttachmentArguments.m_dataType,
			textureAttachmentArguments.m_textureParameterSettings,
			GL_TEXTURE_2D);

		glFramebufferTexture2D(GL_FRAMEBUFFER, textureAttachmentArguments.m_attachment, GL_TEXTURE_2D,
		                       m_textures[textureAttachmentArguments.m_attachment]->GetTextureId(), 0);
	}

	// Create and bind renderbuffer attachments
	for (unsigned int i = 0; i < m_renderbufferAttachmentArguments.size(); i++)
	{
		RenderbufferAttachmentArguments renderbufferAttachmentArguments = m_renderbufferAttachmentArguments[i];
		m_renderbuffers[renderbufferAttachmentArguments.m_attachment] = std::make_shared<OpenGLRenderbuffer>(
			m_width, m_height, renderbufferAttachmentArguments.m_internalFormat);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, renderbufferAttachmentArguments.m_attachment, GL_RENDERBUFFER,
		                          m_renderbuffers[renderbufferAttachmentArguments.m_attachment]->GetRenderbufferId());
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "OpenGLFramebuffer|CreateFramebuffer: Framebuffer is not complete!" << '\n';
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
