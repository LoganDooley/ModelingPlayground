#include "OmnidirectionalLightShadowMap.h"

#include "../OpenGLFramebuffer.h"
#include "../../Application/Rendering/Renderers/OpenGLRenderer.h"

#include "glad/glad.h"

OmnidirectionalLightShadowMap::OmnidirectionalLightShadowMap(unsigned int resolution):
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
				                                                             {GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE},
				                                                             {GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE},
				                                                             {GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE},
			                                                             }),
			                                                             .m_textureTarget = GL_TEXTURE_CUBE_MAP,
		                                                             }
	                                                             }),
	                                                             std::vector<RenderbufferAttachmentArguments>());

	m_shadowMapFramebuffer->Bind();
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	m_shadowMapFramebuffer->Unbind();
}

OmnidirectionalLightShadowMap::~OmnidirectionalLightShadowMap()
{
}

void OmnidirectionalLightShadowMap::CaptureShadowMap(const glm::vec3& lightPosition,
                                                     const OpenGLRenderer* openGLRenderer)
{
	m_shadowMapFramebuffer->Bind();
	glViewport(0, 0, m_resolution, m_resolution);
	glClear(GL_DEPTH_BUFFER_BIT);
	openGLRenderer->RenderOmnidirectionalShadow(lightPosition);
}
