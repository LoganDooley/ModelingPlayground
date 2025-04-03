#include "UnidirectionalLightShadowMap.h"

#include "../OpenGLFramebuffer.h"
#include "../OpenGLRenderer.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../Libraries/stb/stb_image_write.h"

#include "glad/glad.h"

UnidirectionalLightShadowMap::UnidirectionalLightShadowMap(unsigned int resolution):
	m_resolution(resolution)
{
	m_shadowMapFramebuffer = std::make_shared<OpenGLFramebuffer>(m_resolution, m_resolution,
	                                                             std::vector<TextureAttachmentArguments>({
		                                                             {
			                                                             .m_attachment = GL_DEPTH_ATTACHMENT,
			                                                             .m_internalFormat = GL_DEPTH_COMPONENT,
			                                                             .m_format = GL_DEPTH_COMPONENT,
			                                                             .m_dataType = GL_FLOAT,
			                                                             .m_textureParameterSettings = std::vector<
				                                                             TextureParameterSetting>({
				                                                             {GL_TEXTURE_MIN_FILTER, GL_NEAREST},
				                                                             {GL_TEXTURE_MAG_FILTER, GL_NEAREST},
				                                                             {GL_TEXTURE_WRAP_S, GL_REPEAT},
				                                                             {GL_TEXTURE_WRAP_T, GL_REPEAT}
			                                                             })
		                                                             }
	                                                             }),
	                                                             std::vector<RenderbufferAttachmentArguments>());

	m_shadowMapFramebuffer->Bind();
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	m_shadowMapFramebuffer->Unbind();
}

UnidirectionalLightShadowMap::~UnidirectionalLightShadowMap()
{
}

void UnidirectionalLightShadowMap::CaptureShadowMap(const glm::mat4& lightMatrix,
                                                    OpenGLRenderer* openGLRenderer)
{
	glViewport(0, 0, m_resolution, m_resolution);
	m_shadowMapFramebuffer->Bind();
	glClear(GL_DEPTH_BUFFER_BIT);
	openGLRenderer->RenderUnidirectionalShadow(lightMatrix);
}

void UnidirectionalLightShadowMap::DebugCaptureShadowMap(const std::string& filePath)
{
	m_shadowMapFramebuffer->Bind();
	std::vector<GLfloat> depthData(m_resolution * m_resolution);
	glReadPixels(0, 0, m_resolution, m_resolution, GL_DEPTH_COMPONENT, GL_FLOAT, depthData.data());
	std::vector<unsigned char> pixels(m_resolution * m_resolution);
	for (int i = 0; i < m_resolution * m_resolution; i++)
	{
		pixels[i] = static_cast<unsigned char>(depthData[i] * 255.0f);
	}
	stbi_write_png(filePath.c_str(), m_resolution, m_resolution, 1, pixels.data(),
	               m_resolution);
	m_shadowMapFramebuffer->Unbind();
}
