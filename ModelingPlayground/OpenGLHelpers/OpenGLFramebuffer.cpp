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
		m_textures.push_back(std::make_shared<OpenGLTexture>(m_width, m_height,
		                                                     textureAttachmentArguments.m_internalFormat,
		                                                     textureAttachmentArguments.m_format,
		                                                     textureAttachmentArguments.m_dataType,
		                                                     textureAttachmentArguments.m_textureParameterSettings,
		                                                     GL_TEXTURE_2D));

		glFramebufferTexture2D(GL_FRAMEBUFFER, textureAttachmentArguments.m_attachment, GL_TEXTURE_2D,
		                       m_textures[i]->GetTextureId(), 0);
	}

	// Create and bind renderbuffer attachments
	for (unsigned int i = 0; i < m_renderbufferAttachmentArguments.size(); i++)
	{
		RenderbufferAttachmentArguments renderbufferAttachmentArguments = m_renderbufferAttachmentArguments[i];
		m_renderbuffers.push_back(
			std::make_shared<OpenGLRenderbuffer>(m_width, m_height, renderbufferAttachmentArguments.m_internalFormat));

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, renderbufferAttachmentArguments.m_attachment, GL_RENDERBUFFER,
		                          m_renderbuffers[i]->GetRenderbufferId());
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "OpenGLFramebuffer|CreateFramebuffer: Framebuffer is not complete!" << '\n';
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
