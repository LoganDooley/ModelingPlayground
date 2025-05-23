﻿#include "OpenGLFramebuffer.h"

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
	Bind();

	// Create and bind texture attachments
	for (unsigned int i = 0; i < m_textureAttachmentArguments.size(); i++)
	{
		TextureAttachmentArguments textureAttachmentArguments = m_textureAttachmentArguments[i];
		m_textures[textureAttachmentArguments.m_attachment] = std::make_shared<OpenGLTexture>(m_width, m_height,
			textureAttachmentArguments.m_internalFormat,
			textureAttachmentArguments.m_format,
			textureAttachmentArguments.m_dataType,
			textureAttachmentArguments.m_textureParameterSettings,
			textureAttachmentArguments.m_textureTarget);

		if (textureAttachmentArguments.m_textureTarget == GL_TEXTURE_2D)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, textureAttachmentArguments.m_attachment,
			                       textureAttachmentArguments.m_textureTarget,
			                       m_textures[textureAttachmentArguments.m_attachment]->GetTextureId(), 0);
		}
		else if (textureAttachmentArguments.m_textureTarget == GL_TEXTURE_CUBE_MAP)
		{
			glFramebufferTexture(GL_FRAMEBUFFER, textureAttachmentArguments.m_attachment,
			                     m_textures[textureAttachmentArguments.m_attachment]->GetTextureId(), 0);
		}
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

	Unbind();
}
