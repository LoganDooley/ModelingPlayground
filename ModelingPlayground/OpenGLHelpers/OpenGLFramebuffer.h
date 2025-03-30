﻿#pragma once
#include <memory>
#include <vector>

#include "OpenGLTexture.h"
#include "glad/glad.h"

class OpenGLRenderbuffer;

struct TextureAttachmentArguments
{
	GLenum m_attachment;
	GLint m_internalFormat;
	GLenum m_format;
	GLenum m_dataType;
	std::vector<TextureParameterSetting> m_textureParameterSettings;
};

struct RenderbufferAttachmentArguments
{
	GLenum m_attachment;
	GLenum m_internalFormat;
};

class OpenGLFramebuffer
{
public:
	OpenGLFramebuffer(unsigned int width, unsigned int height,
	                  const std::vector<TextureAttachmentArguments>& textureAttachments,
	                  const std::vector<RenderbufferAttachmentArguments>& renderbufferAttachments);

	~OpenGLFramebuffer();

	void Resize(unsigned int width, unsigned int height);

	void Bind();
	void Unbind();

	GLuint GetFramebufferId() const;

private:
	void DeleteFramebuffer();
	void CreateFramebuffer();

	unsigned int m_width;
	unsigned int m_height;
	std::vector<TextureAttachmentArguments> m_textureAttachmentArguments;
	std::vector<RenderbufferAttachmentArguments> m_renderbufferAttachmentArguments;

	GLuint m_framebufferId;
	std::vector<std::shared_ptr<OpenGLTexture>> m_textures;
	std::vector<std::shared_ptr<OpenGLRenderbuffer>> m_renderbuffers;
};
