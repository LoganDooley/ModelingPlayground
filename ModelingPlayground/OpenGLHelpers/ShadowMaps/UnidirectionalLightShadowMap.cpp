#include "UnidirectionalLightShadowMap.h"

#include "../OpenGLFramebuffer.h"
#include "../../Application/Rendering/Renderers/OpenGLRenderer.h"

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
			                                                             }),
			                                                             .m_textureTarget = GL_TEXTURE_2D,
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
                                                    const OpenGLRenderer* openGLRenderer)
{
	m_shadowMapFramebuffer->Bind();
	glViewport(0, 0, m_resolution, m_resolution);
	glClear(GL_DEPTH_BUFFER_BIT);
	openGLRenderer->RenderUnidirectionalShadow(lightMatrix);
}
